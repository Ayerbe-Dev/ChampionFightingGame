#version 330 core

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_texcoord;

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;

void main() {
	vs_position = vec3(vertex_position.x, vertex_position.y, vertex_position.z);
	vs_color = vec3(vertex_color.x, vertex_color.y, vertex_color.z);
	vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y);

	gl_Position = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.f);
}