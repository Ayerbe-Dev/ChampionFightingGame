#version 330 core
out vec4 FragColor;

in GS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D new_col;
uniform sampler2D old_col;

void main() {
    vec4 tex_col_new = texture(new_col, fs_in.TexCoords) * 0.5; 
    vec4 tex_col_old = texture(old_col, fs_in.TexCoords) * 0.5;
    vec4 col = vec4(tex_col_new + tex_col_old);
    FragColor = col; 
}