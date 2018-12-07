cmake_minimum_required(VERSION 3.13)
project(cstring C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(cstring
        strings.c
        strings.h)
