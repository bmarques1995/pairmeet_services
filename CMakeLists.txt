cmake_minimum_required(VERSION 3.22.0)

project(SampleServer VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE SAMPLE_HDRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE SAMPLE_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")

add_executable(SampleServer ${SAMPLE_SRCS})

