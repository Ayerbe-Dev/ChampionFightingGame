#version 330 core

layout (location = 0) in vec2 v_info;

out vec2 tex_coords;

void main() {
    gl_Position = vec4(v_info, 0.0, 1.0);
    tex_coords = v_info;
} 