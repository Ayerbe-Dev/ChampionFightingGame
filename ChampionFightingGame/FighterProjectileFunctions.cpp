#include "Fighter.h"

void Fighter::init_projectile(int id, GameCoordinate init_pos) {
	projectiles[id]->id = this->id;
	projectiles[id]->pos.x = ((init_pos.x + (pos.x * facing_dir)) * facing_dir) + WINDOW_WIDTH / 2;
	projectiles[id]->pos.y = (init_pos.y - WINDOW_HEIGHT) * -1.0 - this->pos.y;

	projectiles[id]->facing_right = facing_right;
	projectiles[id]->facing_dir = facing_dir;
	projectiles[id]->change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Fighter::destroy_projectile(int id) {
	projectiles[id]->id = -1;
}