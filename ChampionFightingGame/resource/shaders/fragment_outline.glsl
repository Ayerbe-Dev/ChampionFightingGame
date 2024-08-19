#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D g_position;

void main() {
	float drawn = texture(g_position, TexCoords).a;

	if (drawn != 1.0) {
	    vec2 texelSize = 1.0 / vec2(textureSize(g_position, 0));
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				vec2 offset = vec2(float(x), float(y)) * texelSize;
				if (texture(g_position, TexCoords + offset).a == 1.0) {
					FragColor = vec4(0.0, 0.0, 0.0, 1.0);
					return;
				}
			}
		}
	}
	FragColor = vec4(0.0);
}