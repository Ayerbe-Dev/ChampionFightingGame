#include "Projectile.h"
#include "Fighter.h"

void Projectile::status_default() {

}

void Projectile::enter_status_default() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_local_param_int("health");
}

void Projectile::exit_status_default() {

}

void Projectile::status_move() {
	add_pos(get_local_param_float("move_x_speed") * facing_dir, 0);
}

void Projectile::enter_status_move() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	change_anim("move");
}

void Projectile::exit_status_move() {

}

void Projectile::status_hit() {
	if (is_anim_end) {
		active = false;
	}
}

void Projectile::enter_status_hit() {
	change_anim("hit");
}

void Projectile::exit_status_hit() {

}