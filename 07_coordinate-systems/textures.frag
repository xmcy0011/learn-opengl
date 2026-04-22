// 片段着色器

#version 430 core

layout(location = 0) in vec2 TexCoord;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D ourTexture; // 纹理采样器

void main() {
    // 只使用颜色
    // FragColor = vec4(ourColor, 1.0);
    // 只使用纹理
    FragColor = texture(ourTexture, TexCoord);
    // 混合
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
}