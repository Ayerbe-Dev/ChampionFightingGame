#version 330 core
out float FragColor;
  
in vec2 TexCoords;
  
uniform sampler2D f_texture;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(f_texture, 0));
    float result = 0.0;
    for (int x = -1; x < 1; ++x) {
        for (int y = -1; y < 1; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(f_texture, TexCoords + offset).r;
        }
    }
    FragColor = result / (2.0 * 2.0);
}