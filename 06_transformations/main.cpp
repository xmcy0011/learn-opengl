#include <cstddef>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  // 初始化 glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(800, 600, "LearnGL", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // 设置当前窗口为 OpenGL 上下文
  glfwMakeContextCurrent(window);

  // 使用 glew 初始化 OpenGL 函数指针（相当于初始化 opengl）
  if (auto result = glewInit(); result != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(result)
              << std::endl;
    return -1;
  }

  // 初始化 opengl 渲染窗口大小
  glViewport(0, 0, 800, 600);
  // 窗口大小变化事件
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 使用归一化坐标，初始化矩形形
  float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  // case1: 创建一个顶点缓冲对象（GPU 内存中，即显存），VBO
  unsigned int vbo;
  glGenBuffers(1, &vbo);              // 生成一个顶点缓冲对象
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // 绑定顶点缓冲对象
  // 复制顶点数据到缓冲区
  // GL_STATIC_DRAW:
  // 数据仅设置一次，但可多次使用。这里的三角形数据不会改变，故使用这个枚举
  // GL_DYNAMIC_DRAW: 数据仅设置一次，GPU 最多使用几次
  // GL_STREAM_DRAW: 数据变化频繁，使用次数很多
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // case1.5: 加载纹理图片
  int width, height, nrChannels;
  auto *imgData = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  if (imgData == nullptr) {
    std::cerr << "Failed to load texture" << std::endl;
    glfwTerminate();
    return -1;
  }
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping/filtering options (on the currently bound texture
  // object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // 超出 [0, 1]，图像重复
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR); // 纹理缩小过滤器
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR); // 纹理放大过滤器
  // 设置纹理环绕、过滤方式
  // 纹理目标：GL_TEXTURE_2D
  // 纹理的 mipmap 级别：0，使用基本级别
  // 纹理格式：GL_RGB，使用3个颜色通道
  // 纹理宽度：width
  // 纹理高度：height
  // 边界宽度：0
  // 格式：GL_RGB，使用3个颜色通道
  // 类型：GL_UNSIGNED_BYTE，使用8位无符号整数
  // 数据：data，纹理数据
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, imgData);
  // 生成必要的 mipmap
  glGenerateMipmap(GL_TEXTURE_2D);
  // 释放图像数据
  stbi_image_free(imgData);

  // case2: 编译着色器和创建着色器程序
  Shader shader("textures.vert", "textures.frag");

  // case3: 生成 VAO
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  // 顶点着色器中，使用 layout = 0 指定的 aPos
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // 启用顶点属性
  // 顶点着色器中，使用 layout = 1 指定的 aColor
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1); // 启用属性
  // 顶点着色器，使用 layout = 2 指定的 aTexCoord
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2); // 启用属性

  // case3.5: 生成 EBO（Element Buffer Objects，元数据缓冲对象），必须在 VAO
  // 之后
  unsigned int ebo;
  glGenBuffers(1, &ebo); // 生成一个元数据缓冲对象
  // copy our index array in a element buffer for OpenGL to use
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // 启用着色器程序
  shader.use();

  // 静态变换，仅一次
  // 设置变换矩阵（反着看，先缩放 0.5，再绕 z 轴旋转 90 度）
  // glm::mat4 transform = glm::mat4(1.0f);
  // transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f,
  // 0.0f, 1.0f)); transform = glm::scale(transform, glm::vec3(0.5f, 0.5f,
  // 0.5f)); unsigned int transformLoc =
  // glGetUniformLocation(shader.shaderProgramID, "transform");
  // // 设置变换矩阵，传递给顶点着色器
  // // 参数2：矩阵个数
  // // 参数3：是否转置，GL_FALSE 表示不转置(交换列和行)，列优先排序这是 GLM
  // // 的默认矩阵布局，因此无需转置矩阵 参数4：变换矩阵的值，使用
  // glm::value_ptr
  // // 转换为 float 数组
  // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 处理 esc 按键退出
    processInput(window);

    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
    glClear(GL_COLOR_BUFFER_BIT);         // 清除颜色缓冲区

    // 随时间变换：先旋转，再平移到右下角（看起来像是在右下角旋转一样？）
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)glfwGetTime(),glm::vec3(0.0f, 0.0f, 1.0f));
    unsigned int transformLoc =
        glGetUniformLocation(shader.shaderProgramID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    // case4: 渲染
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // 交换本次渲染迭代期间用于渲染的颜色缓冲区（一个大型 2D 缓冲区，其中包含
    // GLFW 窗口中每个像素的颜色值），并将其作为输出显示在屏幕上
    glfwSwapBuffers(window);
    // 事件处理
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}