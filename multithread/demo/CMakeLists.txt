cmake_minimum_required(VERSION 3.15)

project(demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT demo)

add_executable(demo main.cpp)