FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    python3 \
    libxml2 \
    upx-ucl \
    build-essential \
    wget \
    xz-utils \
    && rm -rf /var/lib/apt/lists/*

RUN wget -qO /tmp/llvm-mingw.tar.xz https://github.com/mstorsjo/llvm-mingw/releases/download/20240619/llvm-mingw-20240619-ucrt-ubuntu-20.04-x86_64.tar.xz \
    && tar -xf /tmp/llvm-mingw.tar.xz -C /opt \
    && mv /opt/llvm-mingw-20240619-ucrt-ubuntu-20.04-x86_64 /opt/llvm-mingw \
    && rm /tmp/llvm-mingw.tar.xz

# Install xwin and download MSVC/Windows SDK
RUN wget -qO /tmp/xwin.tar.gz https://github.com/Jake-Shadle/xwin/releases/download/0.5.0/xwin-0.5.0-x86_64-unknown-linux-musl.tar.gz \
    && tar -xf /tmp/xwin.tar.gz -C /tmp \
    && /tmp/xwin-0.5.0-x86_64-unknown-linux-musl/xwin --accept-license splat --preserve-ms-arch-notation --output /opt/xwin \
    && rm -rf /tmp/xwin* /root/.xwin-cache

RUN apt-get update && apt-get install -y lld && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
