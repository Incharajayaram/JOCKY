#!/usr/bin/env bash
# One-shot toolchain setup. Run after cloning the repo and placing the toolchain/.
# Ensures all required shared libs are in toolchain/lib so compilation works on
# any Linux distro (Ubuntu 22.04+, 24.04+, Fedora 37+, Debian 12+, etc.)
#
# Strategy per lib:
#   1. Already in toolchain/lib → skip
#   2. Found on the host system → copy it in
#   3. Not on host (e.g. Ubuntu 24.04 ships ICU 74, not 70) → download .deb from
#      Ubuntu 22.04 archive and extract just the .so files (no apt changes)
#
# Hard requirement: glibc 2.35+ (Ubuntu 22.04+). The LLVM binaries were compiled
# against glibc 2.36/2.38 symbols.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
TOOLCHAIN="${TOOLCHAIN_PATH:-$SCRIPT_DIR/../toolchain}"
TOOLCHAIN_LIB="$TOOLCHAIN/lib"

err()  { echo "[ERROR] $*" >&2; exit 1; }
warn() { echo "[WARN]  $*"; }
info() { echo "[INFO]  $*"; }
ok()   { echo "[OK]    $*"; }

[ -d "$TOOLCHAIN_LIB" ] || err "toolchain/lib not found at $TOOLCHAIN_LIB"

# ── glibc version check ───────────────────────────────────────────────────────
GLIBC_VER=$(ldd --version 2>/dev/null | awk 'NR==1{print $NF}')
GLIBC_MINOR=$(echo "$GLIBC_VER" | cut -d. -f2)
if [ "$(echo "$GLIBC_VER" | cut -d. -f1)" -lt 2 ] || [ "$GLIBC_MINOR" -lt 35 ]; then
    err "glibc $GLIBC_VER is too old. Minimum: 2.35 (Ubuntu 22.04+). Cannot run the LLVM toolchain."
fi
info "glibc $GLIBC_VER — OK"

# ── download .deb and extract a .so file ─────────────────────────────────────
# Used only when the lib is not present on the host (e.g. Ubuntu 24.04 w/ ICU 74).
# Downloads from Ubuntu 22.04 (jammy) package archive, no apt changes made.
UBUNTU_ARCHIVE="http://archive.ubuntu.com/ubuntu/pool/main"

fetch_from_deb() {
    local deb_url="$1"
    local so_pattern="$2"   # glob pattern inside the deb, e.g. "*/libicuuc.so.70*"
    local dest_dir="$3"
    local tmpdir
    tmpdir=$(mktemp -d)

    info "Downloading $(basename "$deb_url") ..."
    if command -v curl >/dev/null 2>&1; then
        curl -fsSL "$deb_url" -o "$tmpdir/pkg.deb"
    elif command -v wget >/dev/null 2>&1; then
        wget -q "$deb_url" -O "$tmpdir/pkg.deb"
    else
        err "Neither curl nor wget found. Install one and retry."
    fi

    cd "$tmpdir"
    ar x pkg.deb
    # .deb data archive may be data.tar.xz, data.tar.zst, or data.tar.gz
    local data_archive
    data_archive=$(ls data.tar.* 2>/dev/null | head -1)
    [ -n "$data_archive" ] || err "Could not find data archive inside $deb_url"
    tar -xf "$data_archive" --wildcards "$so_pattern" --strip-components=4 -C "$dest_dir" 2>/dev/null || \
    tar -xf "$data_archive" --wildcards "$so_pattern" -C "$dest_dir" 2>/dev/null || \
        err "Could not extract $so_pattern from $data_archive"

    cd - >/dev/null
    rm -rf "$tmpdir"
}

# ── bundle helper ─────────────────────────────────────────────────────────────
bundle() {
    local soname="$1"       # e.g. libicuuc.so.70
    local realname="$2"     # e.g. libicuuc.so.70.1
    local deb_url="$3"      # fallback: URL to Ubuntu 22.04 .deb
    local so_glob="$4"      # glob to extract from that .deb

    if [ -e "$TOOLCHAIN_LIB/$soname" ]; then
        ok "$soname already in toolchain/lib"
        return
    fi

    # Search standard lib paths on host
    local src=""
    for dir in /lib/x86_64-linux-gnu /lib /usr/lib/x86_64-linux-gnu /usr/lib; do
        if [ -f "$dir/$realname" ]; then
            src="$dir/$realname"; break
        elif [ -L "$dir/$soname" ] && [ -f "$(readlink -f "$dir/$soname")" ]; then
            src="$(readlink -f "$dir/$soname")"; break
        fi
    done

    if [ -n "$src" ]; then
        cp "$src" "$TOOLCHAIN_LIB/$realname"
        [ "$soname" != "$realname" ] && ln -sf "$realname" "$TOOLCHAIN_LIB/$soname"
        ok "bundled $soname from $src"
        return
    fi

    # Not on host — download from Ubuntu archive
    if [ -n "$deb_url" ]; then
        warn "$soname not found on this system — fetching from Ubuntu 22.04 archive"
        fetch_from_deb "$deb_url" "$so_glob" "$TOOLCHAIN_LIB"
        # find what was extracted and create the soname symlink
        local extracted
        extracted=$(ls "$TOOLCHAIN_LIB/$realname" 2>/dev/null || ls "$TOOLCHAIN_LIB/"${soname%%.*}* 2>/dev/null | head -1)
        if [ -n "$extracted" ]; then
            ln -sf "$(basename "$extracted")" "$TOOLCHAIN_LIB/$soname" 2>/dev/null || true
            ok "bundled $soname from Ubuntu 22.04 archive"
        else
            warn "Could not extract $soname — compilation may fail on some inputs"
        fi
    else
        warn "$soname not found and no fallback URL provided"
    fi
}

# Ubuntu 22.04 (jammy) package URLs for version-pinned libs
ICU70_DEB="$UBUNTU_ARCHIVE/i/icu/libicu70_70.1-2_amd64.deb"
LZMA_DEB="$UBUNTU_ARCHIVE/x/xz-utils/liblzma5_5.2.5-2ubuntu1_amd64.deb"
LIBSSL3_DEB="$UBUNTU_ARCHIVE/o/openssl/libssl3_3.0.2-0ubuntu1_amd64.deb"
LIBSTDCXX_DEB="$UBUNTU_ARCHIVE/g/gcc-12/libstdc++6_12.3.0-1ubuntu1~22.04_amd64.deb"

# ── ld.lld — used by clang for Windows PE cross-linking (replaces MinGW ld) ──
bundle_lld() {
    if [ -e "$TOOLCHAIN/bin/ld.lld" ]; then
        ok "ld.lld already in toolchain/bin"
        return
    fi
    # Probe system for any ld.lld (lld-21, lld-19, lld-18, …)
    local src=""
    for candidate in \
        /usr/lib/llvm-21/bin/ld.lld \
        /usr/lib/llvm-20/bin/ld.lld \
        /usr/lib/llvm-19/bin/ld.lld \
        /usr/lib/llvm-18/bin/ld.lld \
        /usr/bin/ld.lld; do
        if [ -f "$candidate" ] || [ -L "$candidate" ]; then
            src="$candidate"; break
        fi
    done
    if [ -n "$src" ]; then
        ln -sf "$src" "$TOOLCHAIN/bin/ld.lld"
        ok "ld.lld -> $src"
        return
    fi
    warn "ld.lld not found — Windows PE linking will fall back to MinGW ld"
    warn "Install lld: sudo apt-get install -y lld"
}

bundle "libicuuc.so.70"   "libicuuc.so.70.1"   "$ICU70_DEB"  "*/libicuuc.so.70*"
bundle "libicudata.so.70" "libicudata.so.70.1" "$ICU70_DEB"  "*/libicudata.so.70*"
bundle "liblzma.so.5"     "liblzma.so.5.2.5"  "$LZMA_DEB"   "*/liblzma.so.5*"
bundle_lld

# libcrypto.so.3 — required by MLIRObfuscationPlugin.so (EVP_blake2b512@OPENSSL_3.0.0)
bundle "libcrypto.so.3" "libcrypto.so.3" "$LIBSSL3_DEB" "*/libcrypto.so.3"

# libstdc++.so.6 — required by both obfuscation plugins; auto-detect version from host
bundle_libstdcxx() {
    if [ -e "$TOOLCHAIN_LIB/libstdc++.so.6" ]; then
        ok "libstdc++.so.6 already in toolchain/lib"
        return
    fi
    local src="" realname=""
    for dir in /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu; do
        if [ -L "$dir/libstdc++.so.6" ]; then
            src="$(readlink -f "$dir/libstdc++.so.6")"
            [ -f "$src" ] && { realname="$(basename "$src")"; break; }
            src=""
        fi
    done
    if [ -n "$src" ]; then
        cp "$src" "$TOOLCHAIN_LIB/$realname"
        ln -sf "$realname" "$TOOLCHAIN_LIB/libstdc++.so.6"
        ok "bundled libstdc++.so.6 ($realname) from $src"
        return
    fi
    warn "libstdc++.so.6 not found on host — fetching from Ubuntu 22.04 archive"
    fetch_from_deb "$LIBSTDCXX_DEB" "*/libstdc++.so.6.0.*" "$TOOLCHAIN_LIB"
    local extracted
    extracted=$(ls "$TOOLCHAIN_LIB"/libstdc++.so.6.0.* 2>/dev/null | head -1)
    if [ -n "$extracted" ]; then
        ln -sf "$(basename "$extracted")" "$TOOLCHAIN_LIB/libstdc++.so.6"
        ok "bundled libstdc++.so.6 from Ubuntu 22.04 archive"
    else
        warn "Could not bundle libstdc++.so.6 — obfuscation plugins may fail to load"
    fi
}
bundle_libstdcxx

# ── MinGW C headers (device-agnostic fix) ────────────────────────────────────
# The toolchain ships with x86_64-w64-mingw32/include/ full of symlinks that
# point to ../../share/mingw-w64/include/.  That share/ directory was itself a
# symlink to an absolute path on the original developer's machine, which breaks
# on every other host.  We fix it here in three steps:
#   1. Replace the absolute symlink with a relative one.
#   2. Populate toolchain/share/mingw-w64/ from the system install or a .deb.
fix_mingw_headers() {
    local share_dir="$TOOLCHAIN/share/mingw-w64"
    local share_link="$TOOLCHAIN/mingw/share/mingw-w64"

    # Step 1 — make the share link relative if it's currently an absolute path
    if [ -L "$share_link" ] && [[ "$(readlink "$share_link")" = /* ]]; then
        rm "$share_link"
        ln -sf "../../share/mingw-w64" "$share_link"
        info "Fixed MinGW share symlink: absolute → relative"
    fi

    # Step 2 — already have real headers under the resolved path
    if [ -f "$share_dir/include/stdio.h" ]; then
        ok "MinGW C headers already present"
        return
    fi

    # Step 3a — use system-installed mingw-w64 headers (mingw-w64-x86-64-dev)
    if [ -f "/usr/x86_64-w64-mingw32/include/stdio.h" ]; then
        mkdir -p "$TOOLCHAIN/share"
        ln -sfn "/usr/x86_64-w64-mingw32" "$share_dir"
        ok "MinGW headers linked from system /usr/x86_64-w64-mingw32"
        return
    fi

    # Step 3b — download mingw-w64-x86-64-dev from Ubuntu archive and extract
    warn "MinGW C headers missing — downloading from Ubuntu archive"
    local deb_url="http://archive.ubuntu.com/ubuntu/pool/universe/m/mingw-w64/mingw-w64-x86-64-dev_11.0.1-3build1_all.deb"
    local tmpdir
    tmpdir=$(mktemp -d)

    if command -v curl >/dev/null 2>&1; then
        curl -fsSL "$deb_url" -o "$tmpdir/pkg.deb"
    else
        wget -q "$deb_url" -O "$tmpdir/pkg.deb"
    fi

    cd "$tmpdir"
    ar x pkg.deb
    local data_archive
    data_archive=$(ls data.tar.* 2>/dev/null | head -1)
    if [ -z "$data_archive" ]; then
        warn "MinGW header download failed — Windows header compilation will fail"
        rm -rf "$tmpdir"; cd - >/dev/null; return
    fi

    mkdir -p "$share_dir"
    # Package layout: ./usr/x86_64-w64-mingw32/include/...
    # Strip 3 components (. usr x86_64-w64-mingw32) → include/... under share_dir
    tar -xf "$data_archive" \
        --wildcards "*/x86_64-w64-mingw32/include/*" \
        --strip-components=3 \
        -C "$share_dir" 2>/dev/null

    cd - >/dev/null
    rm -rf "$tmpdir"

    if [ -f "$share_dir/include/stdio.h" ]; then
        ok "MinGW C headers extracted from Ubuntu package"
    else
        warn "MinGW header extraction may have failed — check $share_dir/include/"
    fi
}
fix_mingw_headers

# ── MinGW GCC runtime symlinks (device-agnostic fix) ─────────────────────────
# toolchain/mingw/lib/gcc/x86_64-w64-mingw32/10-win32/ contains symlinks that
# point to absolute paths on the original developer's machine.  The real files
# live in toolchain/mingw/lib/10-win32/ — we rewire any broken link there to a
# relative path.  Also fixes bin/gcc, bin/g++, bin/ld, bin/ar.
fix_mingw_gcc_libs() {
    local win32_real="$TOOLCHAIN/mingw/lib/10-win32"
    local gcc_win32="$TOOLCHAIN/mingw/lib/gcc/x86_64-w64-mingw32/10-win32"

    if [ -d "$win32_real" ] && [ -d "$gcc_win32" ]; then
        local fixed=0
        while IFS= read -r link; do
            local name
            name=$(basename "$link")
            if [ -e "$win32_real/$name" ]; then
                rm "$link"
                ln -sf "../../../10-win32/$name" "$link"
                fixed=$((fixed + 1))
            fi
        done < <(find "$gcc_win32" -maxdepth 1 -type l ! -exec test -e {} \; -print 2>/dev/null)
        [ "$fixed" -gt 0 ] && ok "Fixed $fixed broken GCC runtime lib symlinks" || ok "GCC runtime lib symlinks OK"
    fi

    # Fix bin/gcc, bin/g++, bin/ld, bin/ar → same-dir versioned binaries
    local mingw_bin="$TOOLCHAIN/mingw/bin"
    for short in gcc g++ ld ar; do
        local link="$mingw_bin/$short"
        local versioned="$mingw_bin/x86_64-w64-mingw32-$short"
        if [ -L "$link" ] && [ ! -e "$link" ] && [ -f "$versioned" ]; then
            rm "$link"
            ln -sf "x86_64-w64-mingw32-$short" "$link"
        fi
    done
}
fix_mingw_gcc_libs

# ── execute permissions ───────────────────────────────────────────────────────
chmod -R +x "$TOOLCHAIN/bin/" "$TOOLCHAIN/mingw/bin/" 2>/dev/null || true
info "execute permissions set"

# ── smoke test ────────────────────────────────────────────────────────────────
export LD_LIBRARY_PATH="$TOOLCHAIN_LIB${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

"$TOOLCHAIN/bin/clang" --version >/dev/null 2>&1 \
    && ok "clang — OK" \
    || err "clang failed. Your glibc ($GLIBC_VER) may be too old (need 2.35+)."

"$TOOLCHAIN/bin/mlir-translate" --version >/dev/null 2>&1 \
    && ok "mlir-translate — OK" \
    || err "mlir-translate failed. Run: ldd $TOOLCHAIN/bin/mlir-translate"

if "$TOOLCHAIN/bin/run-mlir-opt.sh" --load-pass-plugin="$TOOLCHAIN_LIB/MLIRObfuscationPlugin.so" --help >/dev/null 2>&1; then
    ok "MLIRObfuscationPlugin — OK"
else
    warn "MLIRObfuscationPlugin failed to load — check $TOOLCHAIN_LIB/libcrypto.so.3"
fi

"$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc" --version >/dev/null 2>&1 \
    && ok "mingw gcc — OK" \
    || warn "mingw gcc not found (ld.lld is the primary Windows linker)"

if [ -e "$TOOLCHAIN/bin/ld.lld" ]; then
    ok "ld.lld — OK (Windows PE cross-linker ready)"
else
    warn "ld.lld not available — install lld for best cross-compilation support"
fi

[ -f "$TOOLCHAIN/mingw/x86_64-w64-mingw32/include/stdio.h" ] \
    && ok "MinGW C headers — OK" \
    || warn "MinGW C headers not resolving — run setup_toolchain.sh again or install mingw-w64-x86-64-dev"

echo ""
echo "Toolchain is ready."
echo "Minimum system: glibc 2.35+ (Ubuntu 22.04+, Fedora 37+, Debian 12+)."
