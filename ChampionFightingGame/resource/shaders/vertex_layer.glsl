#version 330 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_texcoords;

out vec2 tex_coords;

void main(){
    tex_coords = vec2(v_texcoords);
    gl_Position = vec4(v_pos, 0, 1);
}