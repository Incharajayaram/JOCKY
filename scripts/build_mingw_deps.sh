#!/usr/bin/env bash
# Cross-compile zlib, OpenSSL, and libcurl for x86_64-w64-mingw32 and
# install into toolchain/mingw/x86_64-w64-mingw32/ so the JOCKY build
# pipeline can find them without any system packages.
#
# Run once after cloning:  bash scripts/build_mingw_deps.sh
# Re-run to upgrade:       bash scripts/build_mingw_deps.sh --force

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SYSROOT="$PROJECT_ROOT/toolchain/mingw/x86_64-w64-mingw32"
BUILD_TMP="/tmp/jocky_mingw_deps"
CROSS="x86_64-w64-mingw32"
JOBS="$(nproc 2>/dev/null || echo 4)"

ZLIB_VER="1.3.1"
OPENSSL_VER="3.3.2"
CURL_VER="8.10.1"

FORCE="${1:-}"

# ── helpers ──────────────────────────────────────────────────────────────────

log() { echo "[mingw-deps] $*"; }

need_build() {
    local marker="$SYSROOT/.built_$1"
    [ "$FORCE" = "--force" ] && return 0
    [ -f "$marker" ] && { log "$1 already built (use --force to rebuild)"; return 1; }
    return 0
}

mark_done() { touch "$SYSROOT/.built_$1"; }

check_prereqs() {
    local missing=()
    for cmd in "${CROSS}-gcc" "${CROSS}-ar" "${CROSS}-ranlib" make perl; do
        command -v "$cmd" &>/dev/null || missing+=("$cmd")
    done
    if [ ${#missing[@]} -gt 0 ]; then
        log "ERROR: missing prerequisites: ${missing[*]}"
        log "Install with: sudo apt-get install mingw-w64 perl"
        exit 1
    fi
}

# ── main ─────────────────────────────────────────────────────────────────────

check_prereqs
mkdir -p "$BUILD_TMP" "$SYSROOT/include" "$SYSROOT/lib"
log "Sysroot: $SYSROOT"

# ── zlib ─────────────────────────────────────────────────────────────────────

if need_build "zlib_${ZLIB_VER}"; then
    log "Building zlib $ZLIB_VER..."
    cd "$BUILD_TMP"
    [ -f "zlib-${ZLIB_VER}.tar.gz" ] || \
        curl -fsSL "https://zlib.net/zlib-${ZLIB_VER}.tar.gz" -o "zlib-${ZLIB_VER}.tar.gz"
    rm -rf "zlib-${ZLIB_VER}"
    tar xf "zlib-${ZLIB_VER}.tar.gz"
    cd "zlib-${ZLIB_VER}"

    CC="${CROSS}-gcc" \
    AR="${CROSS}-ar" \
    RANLIB="${CROSS}-ranlib" \
    CFLAGS="-O2 -D_WIN32_WINNT=0x0600" \
        ./configure --prefix="$SYSROOT" --static

    make -j"$JOBS"
    make install
    # Remove the shared-lib stubs configure creates on cross-builds
    rm -f "$SYSROOT/lib/libz.dll.a" "$SYSROOT/lib/libz"*.dll 2>/dev/null || true
    mark_done "zlib_${ZLIB_VER}"
    log "zlib done."
fi

# ── OpenSSL ──────────────────────────────────────────────────────────────────

if need_build "openssl_${OPENSSL_VER}"; then
    log "Building OpenSSL $OPENSSL_VER (this takes a few minutes)..."
    cd "$BUILD_TMP"
    [ -f "openssl-${OPENSSL_VER}.tar.gz" ] || \
        curl -fsSL "https://www.openssl.org/source/openssl-${OPENSSL_VER}.tar.gz" \
             -o "openssl-${OPENSSL_VER}.tar.gz"
    rm -rf "openssl-${OPENSSL_VER}"
    tar xf "openssl-${OPENSSL_VER}.tar.gz"
    cd "openssl-${OPENSSL_VER}"

    ./Configure \
        --prefix="$SYSROOT" \
        --openssldir="$SYSROOT/ssl" \
        --cross-compile-prefix="${CROSS}-" \
        no-shared \
        no-tests \
        no-ui-console \
        no-engine \
        -D_WIN32_WINNT=0x0600 \
        mingw64

    make -j"$JOBS" build_libs
    make install_dev     # headers + static libs only
    mark_done "openssl_${OPENSSL_VER}"
    log "OpenSSL done."
fi

# ── libcurl ──────────────────────────────────────────────────────────────────

if need_build "curl_${CURL_VER}"; then
    log "Building libcurl $CURL_VER..."
    cd "$BUILD_TMP"
    [ -f "curl-${CURL_VER}.tar.gz" ] || \
        curl -fsSL "https://curl.se/download/curl-${CURL_VER}.tar.gz" \
             -o "curl-${CURL_VER}.tar.gz"
    rm -rf "curl-${CURL_VER}"
    tar xf "curl-${CURL_VER}.tar.gz"
    cd "curl-${CURL_VER}"

    ./configure \
        --host="$CROSS" \
        --prefix="$SYSROOT" \
        --with-openssl="$SYSROOT" \
        --disable-shared \
        --enable-static \
        --disable-debug \
        --disable-verbose \
        --disable-ldap \
        --disable-ldaps \
        --without-libidn2 \
        --without-libpsl \
        --without-nghttp2 \
        CFLAGS="-O2 -I${SYSROOT}/include" \
        LDFLAGS="-L${SYSROOT}/lib" \
        PKG_CONFIG_PATH="${SYSROOT}/lib/pkgconfig"

    make -j"$JOBS"
    make install
    mark_done "curl_${CURL_VER}"
    log "libcurl done."
fi

log ""
log "All dependencies installed to: $SYSROOT"
log "  includes : $SYSROOT/include/   (zlib.h, openssl/, curl/)"
log "  libraries: $SYSROOT/lib/       (libz.a, libssl.a, libcrypto.a, libcurl.a)"
log ""
log "Now rebuild the project normally — the pipeline picks them up automatically."
