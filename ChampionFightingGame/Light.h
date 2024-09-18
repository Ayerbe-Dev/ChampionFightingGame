#pragma once
#include <glm/glm.hpp>

class Light {
public:
	Light();
	Light(glm::vec3 pos, glm::vec3 col, float brightness);

	glm::vec3 position;
	glm::vec3 color;
	float brightness;
	bool enabled;
	bool shadow_casting;
	bool changed;
};