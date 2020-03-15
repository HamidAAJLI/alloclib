cmake_minimum_required(VERSION 3.15)
project(allocLib C)

set(CMAKE_C_STANDARD 11)

add_library(allocLib SHARED malloclib.c malloclib.h test.c)