#version 430 core

layout(location = 0) in vec2 TexCoords;

layout(location = 0) out vec4 color;

uniform vec3 textColor;   // 文本颜色，用来调整最终颜色
layout(binding = 0) uniform sampler2D text; // 单颜色通道的字形位图纹理
 
void main()
{    
    // 采样字形位图纹理，只使用红色通道，其他通道为0
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    // 将文本颜色与字形位图纹理相乘，得到最终颜色
    color = vec4(textColor, 1.0) * sampled;
}