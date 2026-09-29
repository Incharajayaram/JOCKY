# JOCKY Compiler - Portable LLVM/MLIR Toolchain Environment
# Uses pre-built toolchain from ./toolchain/
# Supports: Linux ELF, Windows PE (x86-64) cross-compilation

FROM ubuntu:22.04

# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
ENV JOCKY_VERSION=0.1.0
ENV TOOLCHAIN_PATH=/workspace/jocky/toolchain
ENV PATH="${TOOLCHAIN_PATH}/bin:${PATH}"
ENV LD_LIBRARY_PATH="${TOOLCHAIN_PATH}/lib"
ENV PYTHONPATH=/workspace/jocky/src

# Install system dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    git \
    curl \
    wget \
    file \
    unzip \
    openjdk-17-jre-headless \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install MinGW-w64 for Windows cross-compilation
RUN apt-get update && apt-get install -y --no-install-recommends \
    mingw-w64 \
    mingw-w64-tools \
    mingw-w64-x86-64-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install Caddy from GitHub releases
RUN wget -q https://github.com/caddyserver/caddy/releases/download/v2.7.6/caddy_2.7.6_linux_amd64.tar.gz -O /tmp/caddy.tar.gz && \
    tar -xzf /tmp/caddy.tar.gz -C /usr/local/bin caddy && \
    chmod +x /usr/local/bin/caddy && \
    rm /tmp/caddy.tar.gz

# Setup CDN directories
RUN mkdir -p /opt/jocky-cdn/data/uploads && chmod 755 /opt/jocky-cdn /opt/jocky-cdn/data /opt/jocky-cdn/data/uploads

# Copy Caddy config
COPY tools/Caddyfile /opt/jocky-cdn/Caddyfile

# Install Ghidra: use local copy if available, otherwise download
COPY tools/ghidra_install.sh /tmp/ghidra_install.sh
COPY tools/ghidra*.zip* /tmp/ghidra_local/
RUN chmod +x /tmp/ghidra_install.sh && /tmp/ghidra_install.sh && rm -rf /tmp/ghidra_*
ENV PATH="/opt/ghidra/support:${PATH}"

# Install Python dependencies for JOCKY compiler
RUN pip3 install --no-cache-dir \
    click==8.1.6 \
    rich==13.5.2 \
    pyyaml==6.0 \
    tomli==2.0.1

# Setup workspace
RUN mkdir -p /workspace/build /workspace/output

WORKDIR /workspace/jocky

# Copy JOCKY source (including pre-built toolchain)
COPY . .

# Make toolchain and scripts executable
RUN chmod +x ${TOOLCHAIN_PATH}/bin/* 2>/dev/null || true && \
    chmod +x scripts/*.py 2>/dev/null || true

# Install JOCKY as a package
RUN pip3 install --no-cache-dir .

# Verify toolchain is available
RUN ${TOOLCHAIN_PATH}/bin/clang --version && \
    x86_64-w64-mingw32-gcc --version && \
    python3 -c "import jocky; print('JOCKY package OK')" && \
    echo "Toolchain ready"

# Default command
CMD ["/bin/bash"]
