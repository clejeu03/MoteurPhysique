cmake_minimum_required(VERSION 2.8)

project(Waterfall)

# System libraries
find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)

# GLM: math library
include_directories(third-party/glm-0.9.4.6)

# GLEW: to use OpenGL3+ functions
include_directories(third-party/glew-1.10.0/include)
add_definitions(-DGLEW_STATIC=1)
add_subdirectory(third-party/glew-1.10.0)

# Enable C++11 on GCC 4.4+
set(CMAKE_CXX_FLAGS "-std=c++0x")

# Our application
file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.hpp)

add_executable(Waterfall ${SRC_FILES})
target_link_libraries(Waterfall glew ${OPENGL_LIBRARIES} ${SDL_LIBRARY})
