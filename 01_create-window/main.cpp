#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 处理 esc 按键退出
    processInput(window);

    // render command
    // ...

    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
    glClear(GL_COLOR_BUFFER_BIT);         // 清除颜色缓冲区

    // 交换本次渲染迭代期间用于渲染的颜色缓冲区（一个大型 2D 缓冲区，其中包含
    // GLFW 窗口中每个像素的颜色值），并将其作为输出显示在屏幕上
    glfwSwapBuffers(window);
    // 事件处理
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}