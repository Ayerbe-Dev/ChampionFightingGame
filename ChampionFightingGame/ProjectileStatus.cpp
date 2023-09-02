#include "Projectile.h"
#include "Fighter.h"

void Projectile::process_hit() {
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void Projectile::status_activate() {

}

void Projectile::enter_status_activate() {
	active = true;
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_local_param_int("health");
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = 0;
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] = 0;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 0;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 0;
}

void Projectile::exit_status_activate() {

}

void Projectile::status_deactivate() {
	if (is_anim_end) {
		active = false;
	}
}

void Projectile::enter_status_deactivate() {
	active = false;
}

void Projectile::exit_status_deactivate() {

}