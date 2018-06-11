
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.4)
add_definitions(-std=gnu++0x)
set(INCLUDE_FILE main.cpp getFiles.h)
add_executable(demo1 ${INCLUDE_FILE})
