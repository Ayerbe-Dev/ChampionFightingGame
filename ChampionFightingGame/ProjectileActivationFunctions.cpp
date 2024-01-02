#include "Projectile.h"

void Projectile::activate() {
	active = true;
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES] = 0;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 0;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 0;
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("active_frames");
	projectile_int[PROJECTILE_INT_HEALTH] = get_local_param_int("health");
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = 0;
	projectile_flag[PROJECTILE_FLAG_DESPAWN_ON_OOB] = get_local_param_bool("despawn_on_oob");
	unique_activate();
}

void Projectile::deactivate() {
	active = false;
	change_status(PROJECTILE_STATUS_NONE);
	unique_deactivate();
}

void Projectile::unique_activate() {

}

void Projectile::unique_deactivate() {

}