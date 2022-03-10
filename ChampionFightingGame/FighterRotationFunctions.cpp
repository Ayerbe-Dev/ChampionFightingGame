#include "Fighter.h"

void Fighter::set_rot(glm::vec3 rot) {
	extra_rot = rot;
}

void Fighter::add_rot(glm::vec3 rot) {
	extra_rot += rot;
}

void Fighter::reset_rot() {
	extra_rot = glm::vec3(0.0);
}