#version 330 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D f_texture;
uniform vec4 f_colormod;

void main() {
    vec4 tex_col = texture(f_texture, tex_coords);
    tex_col.rgb += f_colormod.rgb;
    tex_col.a -= f_colormod.a;
    FragColor = tex_col;
}