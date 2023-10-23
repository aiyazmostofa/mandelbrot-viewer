cmake_minimum_required(VERSION 3.26)
project(mandelbrot-viewer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(glfw)

add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PRIVATE glad/include)

add_executable(${PROJECT_NAME} WIN32 main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE glad/include)
target_link_libraries(${PROJECT_NAME} glfw glad)
