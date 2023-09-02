#include "Projectile.h"
 
void Projectile::add_pos(glm::vec3 pos) {
	pos *= battle_object_manager->get_world_rate(id);
	this->pos += pos;
}

void Projectile::set_pos(glm::vec3 pos) {
	this->pos = pos;
}