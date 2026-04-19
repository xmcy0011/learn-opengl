cmake_minimum_required(VERSION 3.12)
project(lean-opengl)

# std++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(create-window)
add_subdirectory(hello-triangle)
add_subdirectory(hello-rectangle)