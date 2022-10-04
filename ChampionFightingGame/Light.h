#pragma once
#include <glm/glm.hpp>

class Light {
public:
	Light();
	Light(glm::vec3 pos = glm::vec3(0.0, 1.0, 0.0), glm::vec3 col = glm::vec3(1.0), float brightness = 1.0);
	Light(const Light& other);

	glm::vec3 position;
	glm::vec3 color;

	float linear;
	float quadratic;
	bool enabled;
};