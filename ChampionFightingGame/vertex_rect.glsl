#version 330 core

layout (location = 0) in vec2 v_pos;

uniform mat4 matrix;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * (matrix * vec4(v_pos, 0.0, 1.0));
}