cmake_minimum_required(VERSION 3.12)
project(hello-triangle)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(hello-triangle main.cpp)

target_link_libraries(hello-triangle PRIVATE OpenGL::GL glfw GLEW::GLEW)