add_library(jocky_obfuscation STATIC
    mutation.c
)

target_include_directories(jocky_obfuscation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(NOT WIN32)
    target_compile_options(jocky_obfuscation PRIVATE -fPIC)
endif()

# Test executable
add_executable(mutation_test mutation_test.c mutation.c)
target_include_directories(mutation_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(NOT WIN32)
    target_link_libraries(mutation_test PRIVATE m)
endif()

# Simple test executable
add_executable(simple_test simple_test.c mutation.c)
target_include_directories(simple_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(NOT WIN32)
    target_link_libraries(simple_test PRIVATE m)
endif()

# Enable testing
enable_testing()
add_test(NAME mutation_tests COMMAND mutation_test)
add_test(NAME simple_tests COMMAND simple_test)
