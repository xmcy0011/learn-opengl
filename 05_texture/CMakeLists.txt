cmake_minimum_required(VERSION 3.12)
project(textures)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(textures main.cpp stb_image.h shader.h shader.cpp)

# 拷贝 shader 文件到 Debug 目录
file(COPY textures.vert textures.frag container.jpg DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")

target_link_libraries(textures PRIVATE OpenGL::GL glfw GLEW::GLEW)