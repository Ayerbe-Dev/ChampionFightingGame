#pragma once
#include <glm.hpp>
#include "Shader.h"
using namespace glm;

class GameRect {
public:
	GameRect();
	GameRect(vec2 c1, vec2 c2);
	void init();
	void init(vec2 c1, vec2 c2);
	void destroy();
	void update_corners(vec2 c1, vec2 c2);
	void attach_shader(Shader* shader);
	void bind_scale(vec3* scale);
	void set_alpha(float alpha);
	void set_rgb(vec3 rgb);
	void set_rgba(vec4 rgba);

	void render();

	Shader* shader;

	vec2 corners[4] = { vec2(0.0) };

	vec3 *scale = nullptr;
	vec4 rgba = vec4(0.0);
private:
	void update_buffer_data();

	unsigned int VAO;
	unsigned int VBO;
};