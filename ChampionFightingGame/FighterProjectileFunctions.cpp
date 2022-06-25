#include "Fighter.h"
#include "Projectile.h"

void Fighter::activate_projectile(int id, glm::vec3 init_pos) {
	projectiles[id]->active = true;
	projectiles[id]->pos.x = ((init_pos.x + (pos.x * facing_dir)) * facing_dir);
	projectiles[id]->pos.y = init_pos.y + pos.y;
	projectiles[id]->pos.z = init_pos.z + pos.z;

	projectiles[id]->facing_right = facing_right;
	projectiles[id]->facing_dir = facing_dir;
	projectiles[id]->change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Fighter::deactivate_projectile(int id) {
	projectiles[id]->active = false;
}

void Fighter::set_projectile_int(int projectile, int target, int val) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->set_int(target, val);
	}
}

void Fighter::set_projectile_float(int projectile, int target, float val) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->set_float(target, val);
	}
}

void Fighter::set_projectile_flag(int projectile, int target, bool val) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->set_flag(target, val);
	}
}

void Fighter::add_projectile_pos(int projectile, int pos_x, int pos_y) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->add_pos(pos_x, pos_y);
	}
}

void Fighter::add_projectile_pos(int projectile, glm::vec3 pos) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->add_pos(pos);
	}
}

void Fighter::set_projectile_pos(int projectile, int pos_x, int pos_y) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->set_pos(pos_x, pos_y);
	}
}

void Fighter::set_projectile_pos(int projectile, glm::vec3 pos) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->set_pos(pos);
	}
}

void Fighter::change_projectile_status(int projectile, unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (projectiles[projectile]->active) {
		projectiles[projectile]->change_status(new_status_kind, call_end_status, require_different_status);
	}
}