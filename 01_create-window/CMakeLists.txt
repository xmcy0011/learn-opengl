cmake_minimum_required(VERSION 3.12)
project(create-window)

# opengl 本体
find_package(OpenGL REQUIRED)
# glfw：跨平台窗口管理库
find_package(glfw3 REQUIRED)
# GLEW：OpenGL 扩展库，简化 OpenGL 函数调用
find_package(GLEW REQUIRED)

add_executable(create-window main.cpp)

target_link_libraries(create-window PRIVATE OpenGL::GL glfw GLEW::GLEW)