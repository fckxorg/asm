cmake_minimum_required(VERSION 3.15)
project(compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(compiler main.cpp string_funcs.cpp string_funcs.h reference.h)