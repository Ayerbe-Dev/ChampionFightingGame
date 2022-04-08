#include "Fighter.h"
#include "Projectile.h"

void Fighter::init_projectile(int id, glm::vec3 init_pos) {
	projectiles[id]->active = true;
	projectiles[id]->pos.x = ((init_pos.x + (pos.x * facing_dir)) * facing_dir);
	projectiles[id]->pos.y = init_pos.z + pos.y;
	projectiles[id]->pos.z = init_pos.z + pos.z;

	projectiles[id]->facing_right = facing_right;
	projectiles[id]->facing_dir = facing_dir;
	projectiles[id]->change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Fighter::destroy_projectile(int id) {
	projectiles[id]->active = false;
}