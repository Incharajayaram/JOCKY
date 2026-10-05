#!/usr/bin/env bash
# Patches a distributed toolchain/ to be self-contained on the current host.
# Run once after extracting the toolchain package.
#
# What this does:
#   - Bundles libicuuc.so.70 / libicudata.so.70  (missing on Ubuntu 24.04+ which ships ICU 74)
#   - Bundles liblzma.so.5                        (occasionally missing on minimal installs)
#   - Verifies all tools execute cleanly
#
# Hard requirement: glibc 2.35+ (Ubuntu 22.04+, Fedora 37+, Debian 12+).
# The LLVM toolchain was compiled against glibc 2.36/2.38 symbols.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
TOOLCHAIN="${TOOLCHAIN_PATH:-$SCRIPT_DIR/../toolchain}"
TOOLCHAIN_LIB="$TOOLCHAIN/lib"

err()  { echo "[ERROR] $*" >&2; exit 1; }
info() { echo "[INFO]  $*"; }
ok()   { echo "[OK]    $*"; }

[ -d "$TOOLCHAIN_LIB" ] || err "toolchain/lib not found at $TOOLCHAIN_LIB"

# ── glibc version check ──────────────────────────────────────────────────────
GLIBC_VER=$(ldd --version 2>/dev/null | awk 'NR==1{print $NF}')
GLIBC_MAJOR=$(echo "$GLIBC_VER" | cut -d. -f1)
GLIBC_MINOR=$(echo "$GLIBC_VER" | cut -d. -f2)
if [ "$GLIBC_MAJOR" -lt 2 ] || { [ "$GLIBC_MAJOR" -eq 2 ] && [ "$GLIBC_MINOR" -lt 35 ]; }; then
    err "glibc $GLIBC_VER too old. Minimum required: glibc 2.35 (Ubuntu 22.04+). Found: $GLIBC_VER"
fi
info "glibc $GLIBC_VER — OK"

# ── bundle helper ─────────────────────────────────────────────────────────────
bundle() {
    local soname="$1"         # e.g. libicuuc.so.70
    local realname="$2"       # e.g. libicuuc.so.70.1 (actual file, may equal soname)

    if [ -e "$TOOLCHAIN_LIB/$soname" ]; then
        ok "$soname already in toolchain/lib"
        return
    fi

    # Search standard lib paths
    local src
    for dir in /lib/x86_64-linux-gnu /lib /usr/lib/x86_64-linux-gnu /usr/lib; do
        if [ -f "$dir/$realname" ]; then
            src="$dir/$realname"; break
        elif [ -f "$dir/$soname" ]; then
            src="$(readlink -f "$dir/$soname")"; break
        fi
    done

    if [ -z "$src" ]; then
        echo "[WARN]  $soname not found on this system — skipping (may cause issues on other hosts)"
        return
    fi

    cp "$src" "$TOOLCHAIN_LIB/$realname"
    ln -sf "$realname" "$TOOLCHAIN_LIB/$soname"
    ok "bundled $soname from $src"
}

bundle "libicuuc.so.70"   "libicuuc.so.70.1"
bundle "libicudata.so.70" "libicudata.so.70.1"
bundle "liblzma.so.5"     "liblzma.so.5.2.5"

# ── execute permission check ──────────────────────────────────────────────────
for bin in \
    "$TOOLCHAIN/bin/clang" \
    "$TOOLCHAIN/bin/mlir-translate" \
    "$TOOLCHAIN/bin/mlir-opt" \
    "$TOOLCHAIN/bin/opt" \
    "$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc"
do
    if [ -f "$bin" ] && [ ! -x "$bin" ]; then
        chmod +x "$bin"
        info "fixed permissions: $(basename $bin)"
    fi
done
chmod -R +x "$TOOLCHAIN/bin/" "$TOOLCHAIN/mingw/bin/" 2>/dev/null || true

# ── smoke test ────────────────────────────────────────────────────────────────
export LD_LIBRARY_PATH="$TOOLCHAIN_LIB:$LD_LIBRARY_PATH"

"$TOOLCHAIN/bin/clang" --version > /dev/null 2>&1 \
    && ok "clang — OK" || err "clang failed to execute. Your glibc may be too old."

"$TOOLCHAIN/bin/mlir-translate" --version > /dev/null 2>&1 \
    && ok "mlir-translate — OK" || err "mlir-translate failed. Check toolchain/lib for missing .so files."

"$TOOLCHAIN/mingw/bin/x86_64-w64-mingw32-gcc" --version > /dev/null 2>&1 \
    && ok "mingw gcc — OK" || err "mingw gcc failed."

echo ""
echo "Toolchain is ready. Minimum system: glibc 2.35+ (Ubuntu 22.04+)."
