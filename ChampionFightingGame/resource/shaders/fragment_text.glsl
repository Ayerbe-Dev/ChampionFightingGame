#version 330 core
in vec2 tex_coords;
out vec4 FragColor;

uniform sampler2D f_texture;
uniform vec3 f_colormod;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(f_texture, tex_coords).r);
    vec4 tex_col = vec4(f_colormod.rgb, 1.0);
    FragColor = tex_col * sampled;
}