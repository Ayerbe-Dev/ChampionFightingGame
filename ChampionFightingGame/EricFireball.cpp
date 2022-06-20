#include "EricFireball.h"
#include "EricFireballConstants.h"

EricFireball::EricFireball() {}

EricFireball::EricFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	projectile_name = "eric_fireball";
	resource_dir = "resource/projectile/eric_fireball";
	projectile_int.resize(PROJECTILE_ERIC_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ERIC_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ERIC_FIREBALL_FLAG_MAX, false);
	load_params();
	load_move_scripts();
	loadEricFireballStatusFunctions();
	super_init();
}

void EricFireball::loadEricFireballStatusFunctions() {
	status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
}

void EricFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("move", [this]() {
		execute_frame(0, [this]() {
			push_function(&Projectile::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, glm::vec2(-50.0, -35.0), glm::vec2(50.0, 35.0), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, 16, 16, 10, 10, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, true, KO_KIND_CHIP, true, 10.0, 0.0, 0.0, 1.0);
		});
	});
}

void EricFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void EricFireball::status_default() {
	change_status(PROJECTILE_STATUS_MOVE);
}