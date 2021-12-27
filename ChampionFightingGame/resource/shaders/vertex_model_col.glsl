#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_col;
layout (location = 2) in vec2 v_texcoord;

out vec3 color_offset;
out vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(v_pos, 1.0);
    color_offset = v_col;
    tex_coord = vec2(v_texcoord.x, v_texcoord.y);
}