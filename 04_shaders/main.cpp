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

  // case2: 创建顶点着色器
  const char *vetexShaderSource =
      "#version 430 core\n"
      "layout (location = 0) in vec3 aPos;\n"   // 顶点位置，输入变量
      "layout (location = 1) in vec3 aColor;\n" // 顶点颜色
      "out vec3 ourColor;\n"                    // 输出颜色
      "void main()\n"
      "{\n"
      // 将顶点位置设置为归一化坐标
      // gl_Position 全局输出变量
      "gl_Position = vec4(aPos, 1.0);\n"
      "ourColor = aColor;\n"
      "}\n";
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vetexShaderSource,
                 NULL);          // 将着色器源码附加到着色器对象上
  glCompileShader(vertexShader); // 编译着色器代码
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 检查编译结果
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  // case3: 创建片段着色器
  unsigned int fragmentShader =
      glCreateShader(GL_FRAGMENT_SHADER); // 创建片段着色器对象
  const char *fragmentShaderSource = "#version 430 core\n"
                                     "out vec4 FragColor;\n"
                                     "in vec3 ourColor;\n" // 输入变量，颜色
                                     "void main() {\n"
                                     "FragColor = vec4(ourColor, 1.0);\n"
                                     "}";
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader); // 编译着色器代码
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  // case4: 创建着色器程序
  unsigned int shaderProgram = glCreateProgram();          // 创建着色器程序对象
  glAttachShader(shaderProgram, vertexShader);             // 附加顶点着色器
  glAttachShader(shaderProgram, fragmentShader);           // 附加片段着色器
  glLinkProgram(shaderProgram);                            // 链接着色器程序
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); // 检查链接结果
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }
  glUseProgram(shaderProgram);  // 使用着色器程序
  glDeleteShader(vertexShader); // 删除顶点着色器，程序对象创建成功后，不需要了
  glDeleteShader(fragmentShader); // 删除片段着色器，同上

  // case5: 生成 VAO
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
  glUseProgram(shaderProgram);

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 处理 esc 按键退出
    processInput(window);

    // render command
    // ...

    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除颜色
    glClear(GL_COLOR_BUFFER_BIT);         // 清除颜色缓冲区

    // case7: 渲染
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