#version 330 core
out float FragColor;
  
in vec2 TexCoords;
  
uniform sampler2D ssao;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ssao, 0));
    float result = 0.0;
    for (int x = -1; x < 1; ++x) {
        for (int y = -1; y < 1; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssao, TexCoords + offset).r;
        }
    }
    FragColor = result / (2.0 * 2.0);
}