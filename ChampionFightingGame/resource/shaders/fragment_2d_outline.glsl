#version 330 core
in vec2 tex_coords;
out vec4 FragColor;

uniform sampler2D f_texture;
uniform vec2 border_offset;
uniform vec3 border_color;

void main() {
	FragColor = vec4(0.0);

	FragColor += texture(f_texture, vec2(tex_coords.x, tex_coords.y - border_offset.y));
	FragColor += texture(f_texture, vec2(tex_coords.x - border_offset.x, tex_coords.y));
	FragColor += texture(f_texture, vec2(tex_coords.x + border_offset.x, tex_coords.y));
	FragColor += texture(f_texture, vec2(tex_coords.x, tex_coords.y + border_offset.y));
	FragColor -= texture(f_texture, vec2(tex_coords.x, tex_coords.y)) * 4.0f;

	if (FragColor.a != 0.0) {
		FragColor = vec4(border_color.rgb, 1.0);
	}
}