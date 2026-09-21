cmake_minimum_required(VERSION 3.20)
project(jockyc CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM from the obfuscation build
set(LLVM_ROOT "/home/kamini/projects/llvm-build-obfuscation" CACHE PATH "LLVM build directory")
list(APPEND CMAKE_PREFIX_PATH "${LLVM_ROOT}/lib/cmake/llvm")

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
    HIPStdPar
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
