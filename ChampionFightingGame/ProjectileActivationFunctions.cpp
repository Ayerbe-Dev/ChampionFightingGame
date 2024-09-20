#include "Projectile.h"

void Projectile::activate() {
	active = true;
	object_int[PROJECTILE_INT_ELAPSED_FRAMES] = 0;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 0;
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 0;
	object_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("active_frames");
	object_int[PROJECTILE_INT_HEALTH] = get_param_int("health");
	object_int[PROJECTILE_INT_OWNER_ENDLAG] = 0;
	object_flag[PROJECTILE_FLAG_DESPAWN_ON_OOB] = get_param_bool("despawn_on_oob");
	set_alpha(255);
	unique_activate();
}

void Projectile::deactivate() {
	active = false;
	change_status(BATTLE_OBJECT_STATUS_NONE);
	set_alpha(0, 10);
	unique_deactivate();
}

void Projectile::unique_activate() {

}

void Projectile::unique_deactivate() {

}