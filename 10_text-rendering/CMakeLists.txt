cmake_minimum_required(VERSION 3.12)
project(text-rendering)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_executable(text-rendering main.cpp shader.h shader.cpp)

# 拷贝 shader 文件到 Debug 目录
file(COPY textures.vert textures.frag DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Debug")

target_link_libraries(text-rendering PRIVATE OpenGL::GL glfw GLEW::GLEW glm::glm Freetype::Freetype)