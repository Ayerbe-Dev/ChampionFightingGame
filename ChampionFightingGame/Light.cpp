#include "Light.h"

Light::Light(glm::vec3 pos) {
	position = pos;
	ambient = glm::vec3(0.2, 0.2, 0.2);
	diffuse = glm::vec3(0.5, 0.5, 0.5);
	specular = glm::vec3(1.0, 1.0, 1.0);
	constant = 1.0;
	linear = 0.09;
	quadratic = 0.032;
	enabled = true;
}