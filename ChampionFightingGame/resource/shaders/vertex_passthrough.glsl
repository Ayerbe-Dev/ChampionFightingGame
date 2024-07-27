#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texcoords;

out vec2 TexCoords;

void main() {
    TexCoords = v_texcoords;
    gl_Position = vec4(v_pos, 1.0);
}