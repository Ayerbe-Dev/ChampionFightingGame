#include "Light.h"

Light::Light() {
	position = glm::vec3(0.0);
	color = glm::vec3(1.0);
	linear = 0.07;
	quadratic = 0.017;
	enabled = true;
}

Light::Light(glm::vec3 pos, glm::vec3 col, float brightness) {
	this->position = pos;
	this->color = col * glm::vec3(brightness);

	linear = 0.07;
	quadratic = 0.017;
	enabled = true;
}