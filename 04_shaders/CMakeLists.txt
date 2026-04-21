cmake_minimum_required(VERSION 3.12)
project(shaders)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(shaders main.cpp shader.h shader.cpp)

# 拷贝 shader 文件到 Debug 目录
file(COPY shader.vert shader.frag DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")

target_link_libraries(shaders PRIVATE OpenGL::GL glfw GLEW::GLEW)