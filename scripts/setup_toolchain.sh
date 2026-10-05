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

bundle "libicuuc.so.70"   "libicuuc.so.70.1"   "$ICU70_DEB"  "*/libicuuc.so.70*"
bundle "libicudata.so.70" "libicudata.so.70.1" "$ICU70_DEB"  "*/libicudata.so.70*"
bundle "liblzma.so.5"     "liblzma.so.5.2.5"  "$LZMA_DEB"   "*/liblzma.so.5*"

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
    || err "mingw gcc failed."

echo ""
echo "Toolchain is ready."
echo "Minimum system: glibc 2.35+ (Ubuntu 22.04+, Fedora 37+, Debian 12+)."
