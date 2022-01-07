#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_texcoords;

uniform mat4 matrix;

out vec2 tex_coords;

void main() {
    gl_Position = matrix * vec4(v_pos, 1.0f);
    TexCoord = vec2(v_texcoords.x, 1.0 - v_texcoords.y);
} 