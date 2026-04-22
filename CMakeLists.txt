cmake_minimum_required(VERSION 3.12)
project(lean-opengl)

# std++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(01_create-window)
add_subdirectory(02_hello-triangle)
add_subdirectory(03_hello-rectangle)
add_subdirectory(04_shaders)
add_subdirectory(05_texture)
add_subdirectory(06_transformations)
add_subdirectory(07_coordinate-systems)