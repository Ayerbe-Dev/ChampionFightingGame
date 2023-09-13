#include "Projectile.h"
#include "Fighter.h"

void Projectile::process_hit() {
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void Projectile::status_none() {

}

void Projectile::enter_status_none() {
	active = false;
}

void Projectile::exit_status_none() {

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
		change_status(PROJECTILE_STATUS_NONE);
	}
}

void Projectile::enter_status_deactivate() {
	change_status(PROJECTILE_STATUS_NONE);
}

void Projectile::exit_status_deactivate() {

}

void Projectile::load_projectile_status_scripts() {
	status_script[PROJECTILE_STATUS_NONE] = &Projectile::status_none;
	enter_status_script[PROJECTILE_STATUS_NONE] = &Projectile::enter_status_none;
	exit_status_script[PROJECTILE_STATUS_NONE] = &Projectile::exit_status_none;

	status_script[PROJECTILE_STATUS_ACTIVATE] = &Projectile::status_activate;
	enter_status_script[PROJECTILE_STATUS_ACTIVATE] = &Projectile::enter_status_activate;
	exit_status_script[PROJECTILE_STATUS_ACTIVATE] = &Projectile::exit_status_activate;

	status_script[PROJECTILE_STATUS_DEACTIVATE] = &Projectile::status_deactivate;
	enter_status_script[PROJECTILE_STATUS_DEACTIVATE] = &Projectile::enter_status_deactivate;
	exit_status_script[PROJECTILE_STATUS_DEACTIVATE] = &Projectile::exit_status_deactivate;
}