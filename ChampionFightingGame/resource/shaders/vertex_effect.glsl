#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texcoords;

uniform mat4 matrix;
uniform mat4 camera_matrix;

out vec2 tex_coords;

void main() {
    gl_Position = camera_matrix * (matrix * vec4(v_pos, 1.0f));
    tex_coords = vec2(v_texcoords.x, v_texcoords.y);
}