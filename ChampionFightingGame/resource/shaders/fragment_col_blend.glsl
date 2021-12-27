#version 330 core
out vec4 frag_color;
  
in vec3 color_offset;
in vec2 tex_coord;

uniform sampler2D tex_src;

void main() {
	frag_color = texture(tex_src, tex_coord) * vec4(color_offset, 1.0);  
}