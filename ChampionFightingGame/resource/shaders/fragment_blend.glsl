#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D newest;
uniform sampler2D oldest;

void main() {
    vec4 tex_col_new = texture(newest, TexCoords) * 0.7; 
    vec4 tex_col_old = texture(oldest, TexCoords) * 0.7;
    vec4 col = vec4(tex_col_new + tex_col_old);
    FragColor = col; 
}