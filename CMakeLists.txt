cmake_minimum_required(VERSION 3.5)
project(gl3_0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lGL -lGLEW -lglfw")

set(SOURCE_FILES main.cpp ShadersTool.cpp ShadersTool.h)
add_executable(gl3_0 ${SOURCE_FILES})