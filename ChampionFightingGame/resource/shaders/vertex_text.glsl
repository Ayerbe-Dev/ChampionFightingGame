#version 330 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_texcoords;

uniform mat4 matrix;
out vec2 tex_coords;

void main() {
    gl_Position = matrix * vec4(v_pos, 0.0f, 1.0f);
    tex_coords = vec2(v_texcoords);
}