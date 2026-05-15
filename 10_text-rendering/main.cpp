#include <algorithm>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"

struct Character {
  GLuint TextureID;   // 字形纹理的ID
  glm::ivec2 Size;    // 字形大小
  glm::ivec2 Bearing; // 从基准线到字形左部/顶部的偏移值
  GLuint Advance;     // 原点距下一个字形原点的距离
};

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void RenderText(Shader &s, std::u32string text, GLfloat x, GLfloat y,
                GLfloat scale, glm::vec3 color, GLuint vao, GLuint vbo,
                std::map<char32_t, Character> &characters) {
  // 激活对应的渲染状态
  s.use();

  // 设置文本颜色
  glUniform3f(glGetUniformLocation(s.shaderProgramID, "textColor"), color.x,
              color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);

  // 遍历文本
  for (auto c : text) {
    auto it = characters.find(c);
    if (it == characters.end())
      continue;
    Character ch = it->second;

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // 对每个字符更新 VBO 数据，使用6个顶点，每个顶点 4 float 向量
    GLfloat vertices[6][4] = {
        {xpos, ypos + h, 0.0, 0.0}, {xpos, ypos, 0.0, 1.0},
        {xpos + w, ypos, 1.0, 1.0}, {xpos + w, ypos + h, 1.0, 0.0},
        {xpos, ypos + h, 0.0, 0.0}, {xpos + w, ypos, 1.0, 1.0},
    };
    // 在四边形上绘制纹理
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    // 不要在 VAO 仍绑定时 glBindBuffer(GL_ARRAY_BUFFER, 0)，否则会清空 VAO 里的 VBO 绑定
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // 更新位置到下一个字形的原点，注意单位是1/64像素
    x += (ch.Advance >> 6) * scale; // 每次移动 x 坐标，按比例缩放
  }

  // 恢复默认状态
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

std::string wstring2string(const std::wstring &wstr) {
  if (wstr.empty())
    return {};

#ifdef _WIN32
  int len =
      WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.size()),
                          nullptr, 0, nullptr, nullptr);
  if (len <= 0)
    return {};

  std::string result(len, '\0');
  WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.size()),
                      result.data(), len, nullptr, nullptr);
  return result;
#else
  // 非 Windows 可改用 std::wstring_convert 或 iconv
  std::string result;
  result.reserve(wstr.size());
  for (wchar_t wc : wstr) {
    if (wc > 0xFF)
      return {};
    result.push_back(static_cast<char>(wc));
  }
  return result;
#endif
}

std::u32string utf8ToUtf32 (const std::string& utf8) {
  std::u32string result;
  result.reserve (utf8.size ());

  size_t i = 0;
  while (i < utf8.size ()) {
      const unsigned char c = static_cast<unsigned char> (utf8[i]);
      char32_t cp = 0;
      size_t extra = 0;

      if (c <= 0x7F) {
          cp = c;
      } else if ((c & 0xE0) == 0xC0) {
          cp = c & 0x1F;
          extra = 1;
      } else if ((c & 0xF0) == 0xE0) {
          cp = c & 0x0F;
          extra = 2;
      } else if ((c & 0xF8) == 0xF0) {
          cp = c & 0x07;
          extra = 3;
      } else {
          result.push_back (U'?');
          ++i;
          continue;
      }

      if (i + extra >= utf8.size ()) {
          result.push_back (U'?');
          break;
      }

      bool valid = true;
      for (size_t j = 1; j <= extra; ++j) {
          const unsigned char cc = static_cast<unsigned char> (utf8[i + j]);
          if ((cc & 0xC0) != 0x80) {
              valid = false;
              break;
          }
          cp = (cp << 6) | (cc & 0x3F);
      }

      result.push_back (valid ? cp : U'?');
      i += extra + 1;
  }

  return result;
}

int main() {
  // 初始化 glfw
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(1920, 1080, "LearnGL", NULL, NULL);
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
  glViewport(0, 0, 1920, 1080);
  // 窗口大小变化事件
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // configure global opengl state
  // -----------------------------
  // glEnable(GL_DEPTH_TEST);

  // 初始化字体
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cerr << "Failed to initialize FreeType" << std::endl;
    return -1;
  }

  FT_Face face;
  if (FT_New_Face(ft, wstring2string(L"C:/Windows/Fonts/msyh.ttc").c_str(), 0,
                  &face)) {
    std::cerr << "Failed to load font" << std::endl;
    return -1;
  }
  // 设置字体大小，宽度0代表自动计算，高度32像素
  FT_Set_Pixel_Sizes(face, 0, 32);

  // 初始化字符映射
  std::map<char32_t, Character> characters;

  // 定义需要渲染的文字(Unicode 字符串)
  std::u32string text = utf8ToUtf32("Hello, World! 你好");

  // 遍历文字，加载字符
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 禁用字节对齐限制
  for (char32_t c : text) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cerr << "Failed to load Glyph" << std::endl;
      continue;
    }
    // 生成纹理 GL_TEXTURE_2D
    // glyph 常见属性
    // width: face->glyph->bitmap.width 位图宽度
    // height: face->glyph->bitmap.rows 位图高度
    // bearingX: face->glyph->bitmap_left
    // 水平距离，即位图相对于原点的水平位置（像素） bearingY:
    // face->glyph->bitmap_top 垂直距离，即位图相对于原点的垂直位置（像素）
    // advance: face->glyph->advance.x
    // 原点距下一个字形原点的水平距离（单位：1/64像素）
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // internalformat & format: GL_RED，使用单通道红色纹理 8位灰度图
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 储存字符供之后使用
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<GLuint>(face->glyph->advance.x)};
    characters[c] = character;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  // 创建 VBO 和 VAO 来渲染4边形
  GLuint vbo, vao;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // 每个四边形需要6个顶点，每个顶点 4 float 向量
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr,
               GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 创建和编译着色器程序
  Shader shader("textures.vert", "textures.frag");

  // 正交投影，左下角为原点
  const glm::mat4 projection = glm::ortho(0.f, 1920.f, 0.f, 1080.f);
  const GLfloat textX = 1500.f;
  const GLfloat textY = 900.f;

  shader.use();
  glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgramID, "projection"),
                     1, GL_FALSE, glm::value_ptr(projection));

  // 启用混合，处理字体颜色
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // 渲染循环
  while (!glfwWindowShouldClose(window)) {
    // 测试渲染，使用简单的颜色清除
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    // 坐标始终使用设计空间，viewport 负责缩放到实际分辨率
    RenderText(shader, text, textX, textY, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f),
               vao, vbo, characters);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}