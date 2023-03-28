cmake_minimum_required(VERSION 3.20)
project(exp1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
# add subdirectory
#include googletest/include dir
add_subdirectory(lib)
include_directories(lib/googletest/include)

#include the googlemock/include dir
include_directories(lib/googlemock/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(test test_main.cpp)
target_link_libraries(test gtest gtest_main)
target_link_libraries(test gmock gmock_main)


add_executable(exp1 main.cpp)