#include "Projectile.h"

void Projectile::add_pos(float x, float y) {
	add_pos(glm::vec3(x, y, 0));
}
 
void Projectile::add_pos(glm::vec3 pos) {
	pos *= battle_object_manager->get_time_multiplier(id);
	this->pos += pos;
}

void Projectile::set_pos(float x, float y) {
	set_pos(glm::vec3(x, y, 0));
}

void Projectile::set_pos(glm::vec3 pos) {
	this->pos = pos;
}