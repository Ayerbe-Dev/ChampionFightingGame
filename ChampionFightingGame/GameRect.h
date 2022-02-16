#pragma once
#include <glm.hpp>
#include "Shader.h"
using namespace glm;

class GameRect {
public:
	GameRect();
	GameRect(vec2 c1, vec2 c2); //The two corners
	void render();

	Shader* shader;

	vec2 c1 = vec2(0.0, 0.0);
	vec2 c2 = vec2(0.0, 0.0);

	vec3 *scale;
private:
	unsigned int VAO;
	unsigned int VBO;
};