#include "Projectile.h"
 
void Projectile::add_pos(glm::vec3 pos) {
	pos *= object_manager->get_world_rate(this);
	this->pos += pos;
}

void Projectile::set_pos(glm::vec3 pos) {
	this->pos = pos;
}