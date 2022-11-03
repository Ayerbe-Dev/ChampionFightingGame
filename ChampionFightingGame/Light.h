#pragma once
#include <glm/glm.hpp>

class Light {
public:
	Light();
	Light(glm::vec3 pos, glm::vec3 col, float brightness);

	glm::vec3 position;
	glm::vec3 color;

	float linear;
	float quadratic;
	bool enabled;
};