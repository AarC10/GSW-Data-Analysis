cmake_minimum_required(VERSION 3.21)
project(data_analysis)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(data_analysis
        analysis.cpp
        analysis.h)
