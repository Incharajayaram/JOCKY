# Build stage
FROM ubuntu:22.04 as builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-pip \
    python3-dev \
    llvm-14-dev \
    clang-14 \
    libelf-dev \
    libz-dev \
    libbpf-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /build
COPY . .

# Build C++ compiler
RUN mkdir -p compiler/build && \
    cd compiler/build && \
    cmake .. -DLLVM_ROOT=/usr/lib/llvm-14 && \
    make -j$(nproc)

# Install Python dependencies
RUN pip3 install -e .

# Runtime stage
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    llvm-14 \
    clang-14 \
    git \
    gdb \
    strace \
    ltrace \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /jocky

# Copy built compiler from builder
COPY --from=builder /build/compiler/build/jockyc /usr/local/bin/
COPY --from=builder /build . .

# Install Python CLI
RUN pip3 install -e .

# Set up environment
ENV JOCKY_LLVM_TOOLCHAIN=/usr/lib/llvm-14
ENV PATH="/usr/local/bin:${PATH}"

# Create non-root user
RUN useradd -m -s /bin/bash jocky && \
    chown -R jocky:jocky /jocky

USER jocky

ENTRYPOINT ["jocky"]
CMD ["--help"]
