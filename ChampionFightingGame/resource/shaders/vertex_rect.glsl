#version 330 core

layout (location = 0) in vec2 v_pos;

uniform mat4 matrix;
uniform mat4 camera_matrix;

void main() {
    gl_Position = camera_matrix * (matrix * vec4(v_pos, 0.0f, 1.0f));
}