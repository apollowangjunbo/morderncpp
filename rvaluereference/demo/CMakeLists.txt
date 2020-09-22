cmake_minimum_required(VERSION 3.15)

project(rval_ref_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT rval_ref_demo)

add_executable(rval_ref_demo main.cpp)