#include "Light.h"

Light::Light() {
	position = glm::vec3(0.0);
	color = glm::vec3(1.0);
	brightness = 0.0f;
	enabled = true;
}

Light::Light(glm::vec3 pos, glm::vec3 col, float brightness) {
	this->position = pos;
	this->color = col;
	this->brightness = brightness;

	enabled = true;
}