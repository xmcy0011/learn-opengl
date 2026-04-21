// 顶点着色器

#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

layout(location = 0) out vec3 ourColor;
layout(location = 1) out vec2 TexCoord;

layout(location = 0) uniform mat4 transform; // 变换矩阵

void main() {
  gl_Position = transform * vec4(aPos, 1.0);
  ourColor = aColor;
  TexCoord = aTexCoord;
}
