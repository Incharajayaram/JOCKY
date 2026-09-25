cmake_minimum_required(VERSION 3.20)
project(jockyc CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate LLVM.  Resolution order:
#   1. -DLLVM_ROOT=<path>           explicit override
#   2. JOCKY_LLVM_ROOT env var
#   3. <repo-root>/toolchain        the bundled custom build (default)
#   4. llvm-config on PATH          system install fallback

if(NOT LLVM_ROOT)
    if(DEFINED ENV{JOCKY_LLVM_ROOT})
        set(LLVM_ROOT "$ENV{JOCKY_LLVM_ROOT}" CACHE PATH "LLVM build/install directory")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../toolchain/lib/cmake/llvm/LLVMConfig.cmake")
        # Prefer the bundled toolchain (one directory up from compiler/)
        get_filename_component(LLVM_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../toolchain" ABSOLUTE)
        set(LLVM_ROOT "${LLVM_ROOT}" CACHE PATH "LLVM build/install directory")
        message(STATUS "Using bundled toolchain: ${LLVM_ROOT}")
    else()
        find_program(LLVM_CONFIG llvm-config)
        if(LLVM_CONFIG)
            execute_process(
                COMMAND ${LLVM_CONFIG} --cmakedir
                OUTPUT_VARIABLE LLVM_CMAKE_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            if(LLVM_CMAKE_DIR)
                list(APPEND CMAKE_PREFIX_PATH "${LLVM_CMAKE_DIR}")
                message(STATUS "Using llvm-config cmake dir: ${LLVM_CMAKE_DIR}")
            endif()
        endif()
    endif()
endif()

if(LLVM_ROOT)
    list(PREPEND CMAKE_PREFIX_PATH "${LLVM_ROOT}/lib/cmake/llvm")
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM include dirs: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM library dirs: ${LLVM_LIBRARY_DIRS}")

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

add_definitions(${LLVM_DEFINITIONS})

# Collect source files
set(JOCKYC_SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/codegen.cpp
    src/pipeline.cpp
    src/packer.cpp
)

add_executable(jockyc ${JOCKYC_SOURCES})

target_include_directories(jockyc PRIVATE include)

# Link against LLVM components we actually need
llvm_map_components_to_libnames(llvm_libs
    Core
    Support
    Analysis
    TransformUtils
    ScalarOpts
    InstCombine
    AggressiveInstCombine
    IPO
    Passes
    Vectorize
    ObjCARCOpts
    Instrumentation
    Linker
    BitWriter
    IRReader
    Obfuscation
    X86CodeGen
    X86Desc
    X86Info
    X86AsmParser
    MC
    MCParser
    CodeGen
    CodeGenTypes
    SelectionDAG
    GlobalISel
    AsmPrinter
    Target
    TargetParser
    BinaryFormat
    BitstreamReader
    Remarks
    Demangle
    DebugInfoDWARF
    Symbolize
    LineEditor
    Option
    TextAPI
    WindowsManifest
    FrontendOpenMP
    Coroutines
    Coverage
    LTO
    Object
    OrcJIT
    ExecutionEngine
    RuntimeDyld
    AsmParser
    MIRParser
    DWP
    LibDriver
    XRay
    FuzzMutate
    FileCheck
    CFGuard
    Extensions
    IRPrinter
    HipStdPar
    Exegesis
    ExegesisX86
)

target_link_libraries(jockyc ${llvm_libs})

# Platform libs
if(WIN32)
    target_link_libraries(jockyc ntdll psapi)
else()
    target_link_libraries(jockyc dl pthread z tinfo)
endif()
