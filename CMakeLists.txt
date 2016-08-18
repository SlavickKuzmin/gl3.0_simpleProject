cmake_minimum_required(VERSION 3.5)
project(gl3_0)

find_package(ImageMagick COMPONENTS Magick++)

include_directories(${ImageMagick_INCLUDE_DIRS})

LINK_DIRECTORIES(/home/slavickkuzmin/AntTweakBar/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/home/slavickkuzmin/AntTweakBar/include -lAntTweakBar -lassimp -std=c++11 -lGL -lGLEW -lglfw")

set(SOURCE_FILES main.cpp ShadersTool.cpp ShadersTool.h model.cpp model.h Window.cpp Window.h texture.cpp texture.h util.cpp util.h math_3d.cpp math_3d.h types.h)

add_executable(gl3_0 ${SOURCE_FILES})

target_link_libraries(gl3_0 ${ImageMagick_LIBRARIES})