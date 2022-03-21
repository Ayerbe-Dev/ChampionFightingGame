#include "Projectile.h"

void Projectile::status_default() {

}

void Projectile::enter_status_default() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_param_int("health");
}

void Projectile::exit_status_default() {

}

void Projectile::status_move() {
	pos.x += get_param_float("move_x_speed") * facing_dir;
}

void Projectile::enter_status_move() {
	change_anim("move");
}

void Projectile::exit_status_move() {

}

void Projectile::status_hit() {

}

void Projectile::enter_status_hit() {

}

void Projectile::exit_status_hit() {

}