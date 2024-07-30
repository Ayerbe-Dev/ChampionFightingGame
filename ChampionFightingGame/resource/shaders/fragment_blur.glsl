#version 330 core
layout (location=0) out vec4 FragColor;
  
in vec2 TexCoords;
  
uniform sampler2D f_texture;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(f_texture, 0));
    float result = 0.0;
    for (int x = -1; x < 1; x++) {
        for (int y = -1; y < 1; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(f_texture, TexCoords + offset).r;
        }
    }
    FragColor = vec4(result / 4.0, 0.0, 0.0, 1.0);
}