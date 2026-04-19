cmake_minimum_required(VERSION 3.12)
project(hello-rectangle)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(hello-rectangle main.cpp)

target_link_libraries(hello-rectangle PRIVATE OpenGL::GL glfw GLEW::GLEW)