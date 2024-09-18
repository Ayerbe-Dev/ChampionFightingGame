#include "EricFireball.h"
#include "EricFireballConstants.h"

EricFireball::EricFireball() {}

EricFireball::EricFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	projectile_name = "eric_fireball";
	resource_dir = "resource/projectile/eric_fireball";
	object_int.resize(PROJECTILE_ERIC_FIREBALL_INT_MAX, 0);
	object_float.resize(PROJECTILE_ERIC_FIREBALL_FLOAT_MAX, 0.0);
	object_flag.resize(PROJECTILE_ERIC_FIREBALL_FLAG_MAX, false);
	object_flag.resize(PROJECTILE_ERIC_FIREBALL_STRING_MAX, "");
	status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);

	load_projectile();
}

void EricFireball::projectile_unique_main() {
	if (object_int[PROJECTILE_INT_HEALTH] <= 0 && active) {
		if (get_alpha() == 0) {
			deactivate();
		}
		else {
			add_alpha(-50);
		}
	}
}

void EricFireball::unique_activate() {
	owner->object_flag[CHARA_ERIC_FLAG_FIREBALL_ACTIVE] = true;
	if (owner->object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		object_int[PROJECTILE_INT_ATTACK_LEVEL] = 1;
		object_int[PROJECTILE_INT_HEALTH] = 2;
	}
	set_alpha(255);
	change_status(PROJECTILE_ERIC_FIREBALL_STATUS_HOVER);
}

void EricFireball::unique_deactivate() {
	owner->object_flag[CHARA_ERIC_FLAG_FIREBALL_ACTIVE] = false;
}