#version 330 core
layout (location=0) out vec4 FragColor;
  
in vec2 TexCoords;
  
uniform sampler2D f_texture;
uniform int blur_amount = 2;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(f_texture, 0));
    float result = 0.0;
    for (int x = -blur_amount; x < blur_amount; x++) {
        for (int y = -blur_amount; y < blur_amount; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(f_texture, TexCoords + offset).r;
        }
    }
    FragColor = vec4(result / ((blur_amount * 2) * (blur_amount * 2)), 0.0, 0.0, 1.0);
}