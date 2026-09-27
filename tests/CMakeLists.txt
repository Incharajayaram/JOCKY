find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "Google Test not found, skipping tests")
    return()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Test executables
add_executable(test_common test_common.cpp)
target_link_libraries(test_common kernel_evasion GTest::gtest_main)

add_test(NAME CommonTests COMMAND test_common)
