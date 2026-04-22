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

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // 使用归一化坐标，初始化立方体
  float vertices[] = {
      // positions                        // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

// 画 10 个立方体（不自转）
//#define DRAW_MULTIPLE_CUBE
#ifdef DRAW_MULTIPLE_CUBE
  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};
#endif

  // case1: 顶点缓冲对象 VBO、顶点数组对象 VAO
  unsigned int vbo, vao;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);

  glBindVertexArray(vao);

  // 复制顶点数据到缓冲区
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute: aPos
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute: aTexCoord
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // case2: 加载纹理图片
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, imgData);
  // 生成必要的 mipmap
  glGenerateMipmap(GL_TEXTURE_2D);
  // 释放图像数据
  stbi_image_free(imgData);

  // case2: 编译着色器和创建着色器程序
  Shader shader("textures.vert", "textures.frag");
  shader.use();

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 处理 esc 按键退出
    processInput(window);

    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // activate shader
    shader.use();

    // 设置变换矩阵
    #ifndef DRAW_MULTIPLE_CUBE
    glm::mat4 model = glm::mat4(1.0f);      // 模型变换矩阵（LOCAL SPACE -> WORLD SPACE）
    #endif
    glm::mat4 view = glm::mat4(1.0f);       // 视图变换矩阵（WORLD SPACE -> VIEW SPACE）
    glm::mat4 projection = glm::mat4(1.0f); // 投影变换矩阵（VIEW SPACE -> CLIP SPACE）

    #ifndef DRAW_MULTIPLE_CUBE
    // 先绕 x 轴旋转一下平面，让它看起来像是平放在地板上
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    // 随时间旋转
    // 1）角度参数是 glfwGetTime()，单位是秒，但 glm::rotate 这里按弧度用
    // => 角速度约 1 rad/s（约 57.3°/s）
    // 2）
    // - 只绕 X 转 (1,0,0），最容易感知俯仰
    // - 只绕 Y 转 (0,1,0)，像转盘左右转
    // - 只绕 Z 转 (0,0,1)，屏幕内平面旋转（最直观的 2D 转）
    // - 再换回 (0.5,1,0)，就能明显看出是“混合轴”旋转
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    #endif

    // 向后移动=场景沿负 z 轴方向平移
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // 场景使用透视投影，远小近大
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    unsigned int viewLoc = glGetUniformLocation(shader.shaderProgramID, "view");
    unsigned int projectionLoc = glGetUniformLocation(shader.shaderProgramID, "projection");
    
    #ifndef DRAW_MULTIPLE_CUBE
    unsigned int modelLoc = glGetUniformLocation(shader.shaderProgramID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    #else
    // 画 10 立方体
    for(unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i; 

      model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
      unsigned int modelLoc = glGetUniformLocation(shader.shaderProgramID, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    #endif

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));  

    // render box
    glBindVertexArray(vao);
    #ifndef DRAW_MULTIPLE_CUBE
    glDrawArrays(GL_TRIANGLES, 0, 36);
    #endif

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}