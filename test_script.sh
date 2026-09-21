#!/bin/bash
docker run --rm -v "$(pwd)":/workspace jocky-env /bin/bash -c "apt-get update && apt-get install -y clang && /workspace/toolchain/bin/clang -target x86_64-w64-mingw32 -resource-dir /usr/lib/llvm-18/lib/clang/18 -c malware_windows_v3_full.c -o temp.o"
