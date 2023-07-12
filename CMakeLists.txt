cmake_minimum_required(VERSION 3.26)
project(mandelbrot_viewer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(glfw)
add_subdirectory(glad)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

add_executable(mandelbrot-viewer WIN32 main.cpp)
target_link_libraries(mandelbrot-viewer glfw glad)
