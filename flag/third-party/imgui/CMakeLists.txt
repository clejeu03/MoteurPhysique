cmake_minimum_required(VERSION 2.6)

project(imgui)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(
    GLOB_RECURSE
    src_files
    src/*.cpp include/*.h
)

include_directories(../glew-1.10.0/include)

add_library(imgui STATIC ${src_files})
