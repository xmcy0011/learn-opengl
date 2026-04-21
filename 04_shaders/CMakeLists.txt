cmake_minimum_required(VERSION 3.12)
project(shaders)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(shaders main.cpp)

target_link_libraries(shaders PRIVATE OpenGL::GL glfw GLEW::GLEW)