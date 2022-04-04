#pragma once
#include <glm/glm.hpp>

class Light {
public:
	Light(glm::vec3 pos = glm::vec3(0.0, 1.0, 0.0));

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
	bool enabled;
};