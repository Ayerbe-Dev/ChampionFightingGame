#version 330 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D f_texture;
uniform vec4 f_colormod;

void main() {    
    vec4 tex_col = texture(f_texture, tex_coords);
    tex_col.r += f_colormod.x;
    tex_col.g += f_colormod.y;
    tex_col.b += f_colormod.z;
    tex_col.a -= f_colormod.w;
    FragColor = tex_col;
}