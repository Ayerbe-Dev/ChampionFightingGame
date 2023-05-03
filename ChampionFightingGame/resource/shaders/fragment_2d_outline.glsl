#version 330 core
in vec2 tex_coords;
out vec4 FragColor;

uniform sampler2D f_texture;
uniform vec2 border_offset;
uniform vec4 border_color;

void main() {
	FragColor = texture(f_texture, vec2(tex_coords.x, tex_coords.y));

	if (FragColor.a != border_color.a) {
		FragColor += texture(f_texture, vec2(tex_coords.x, tex_coords.y - border_offset.y));
		FragColor += texture(f_texture, vec2(tex_coords.x - border_offset.x, tex_coords.y));
		FragColor += texture(f_texture, vec2(tex_coords.x + border_offset.x, tex_coords.y));
		FragColor += texture(f_texture, vec2(tex_coords.x, tex_coords.y + border_offset.y));
		FragColor += texture(f_texture, vec2(tex_coords.x - border_offset.x / 2.0f, tex_coords.y - border_offset.y / 2.0f));
		FragColor += texture(f_texture, vec2(tex_coords.x - border_offset.x / 2.0f, tex_coords.y + border_offset.y / 2.0f));
		FragColor += texture(f_texture, vec2(tex_coords.x + border_offset.x / 2.0f, tex_coords.y + border_offset.y / 2.0f));
		FragColor += texture(f_texture, vec2(tex_coords.x + border_offset.x / 2.0f, tex_coords.y - border_offset.y / 2.0f));

		if (FragColor.a != 0.0) {
			FragColor = vec4(border_color);
		}
	}
}