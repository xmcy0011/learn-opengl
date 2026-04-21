#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

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

  // 使用归一化坐标，初始化三角形
  // z 深度为0
  float vertices[] = {
      // positions                     // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
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

  // case2: 编译着色器和创建着色器程序
  Shader shader("shader.vert",
                "shader.frag");

  // case3: 生成 VAO
  unsigned int vao;
  glGenVertexArrays(1, &vao); // 生成一个顶点数组对象
  // 1. bind Vertex Array Object
  glBindVertexArray(vao);
  // 顶点着色器中，使用 layout = 0指定的 aPos
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0); // 启用顶点属性
  // 顶点着色器中，使用 layout = 1指定的 aColor
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1); // 启用属性

  // 启用着色器程序
  shader.use();

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 处理 esc 按键退出
    processInput(window);

    // render command
    // ...

    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
    glClear(GL_COLOR_BUFFER_BIT);         // 清除颜色缓冲区

    // case4: 渲染
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // 交换本次渲染迭代期间用于渲染的颜色缓冲区（一个大型 2D 缓冲区，其中包含
    // GLFW 窗口中每个像素的颜色值），并将其作为输出显示在屏幕上
    glfwSwapBuffers(window);
    // 事件处理
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}