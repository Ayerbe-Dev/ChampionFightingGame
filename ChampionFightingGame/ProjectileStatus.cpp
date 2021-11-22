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

void Projectile::loadStatusScripts() {
	status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::status_default;
	enter_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::enter_status_default;
	exit_status_script[PROJECTILE_STATUS_DEFAULT] = &Projectile::exit_status_default;

	status_script[PROJECTILE_STATUS_MOVE] = &Projectile::status_move;
	enter_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::enter_status_move;
	exit_status_script[PROJECTILE_STATUS_MOVE] = &Projectile::exit_status_move;

	status_script[PROJECTILE_STATUS_HIT] = &Projectile::status_hit;
	enter_status_script[PROJECTILE_STATUS_HIT] = &Projectile::enter_status_hit;
	exit_status_script[PROJECTILE_STATUS_HIT] = &Projectile::exit_status_hit;
}