#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D f_texture;
uniform float exposure;

void main() {
    vec3 hdrColor = texture(f_texture, TexCoords).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    FragColor = vec4(result, 1.0);
}