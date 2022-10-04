#include "Light.h"

Light::Light() {

}

Light::Light(glm::vec3 pos, glm::vec3 col, float brightness) {
	this->position = pos;
	this->color = col * glm::vec3(brightness);

	linear = 0.09;
	quadratic = 0.032;
	enabled = true;
}

Light::Light(const Light& other) {
	this->position = other.position;
	this->color = other.color;
	this->linear = other.linear;
	this->quadratic = other.quadratic;
	this->enabled = other.enabled;
}