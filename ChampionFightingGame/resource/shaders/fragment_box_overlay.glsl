#version 330 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D f_texture;

void main() {
    vec4 tex_col = texture(f_texture, tex_coords);
    FragColor = tex_col;
}