#include "Light.h"

Light::Light(glm::vec3 pos) {
	position = pos;
	color = glm::vec3(1.0, 1.0, 1.0);

	linear = 0.09;
	quadratic = 0.032;
	enabled = true;
}