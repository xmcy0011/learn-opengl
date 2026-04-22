// 顶点着色器

#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout(location = 0) out vec2 TexCoord;

layout(location = 0) uniform mat4 model;      // 模型变换矩阵
layout(location = 1) uniform mat4 view;       // 视图变换矩阵
layout(location = 2) uniform mat4 projection; // 投影变换矩阵

void main() {
  gl_Position = projection * view * model * vec4(aPos, 1.0);
  TexCoord = aTexCoord;
}