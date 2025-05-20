cmake_minimum_required(VERSION 3.14)
project(banking)

option(COVERAGE "Enable code coverage" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG " -O0 --coverage")
set(CMAKE_C_FLAGS_DEBUG " -O0 --coverage")

add_library(banking
    banking/Account.cpp
    banking/Transaction.cpp
)

target_include_directories(banking PUBLIC
    
)

if(COVERAGE)
    message(STATUS "Coverage build enabled")
    target_compile_options(banking PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(banking PRIVATE gcov)
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

add_executable(banking_tests
    tests/AccountTest.cpp
    tests/TransactionTest.cpp
)

target_include_directories(banking_tests PRIVATE
    
    
)

target_link_libraries(banking_tests
    PRIVATE
    banking
    gtest
    gmock
    gtest_main
)

if(COVERAGE)
    target_compile_options(banking_tests PRIVATE -fprofile-arcs -ftest-coverage -O0)
    target_link_libraries(banking_tests PRIVATE gcov)
endif()

enable_testing()
add_test(NAME banking_tests COMMAND banking_tests)
