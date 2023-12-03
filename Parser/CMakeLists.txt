cmake_minimum_required(VERSION 3.26)
project(Code)

set(CMAKE_CXX_STANDARD 17)

add_executable(Code parser.cpp
        datetime.h)
