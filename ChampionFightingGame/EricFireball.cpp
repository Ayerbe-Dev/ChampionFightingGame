#include "EricFireball.h"
#include "Battle.h"
#include "EricFireballConstants.h"

EricFireball::EricFireball(int id, PlayerInfo* player_info, FighterAccessor *fighter_accessor) {
	this->player_info = player_info;
	this->fighter_accessor = fighter_accessor;
	this->projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	projectile_name = "eric_fireball";
	resource_dir = "resource/projectile/eric_fireball";
	projectile_int.resize(PROJECTILE_ERIC_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ERIC_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ERIC_FIREBALL_FLAG_MAX, false);
	load_params();
	load_move_scripts();
	loadEricFireballStatusFunctions();
	superInit();
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
		if (is_excute_frame(0)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ -5,35 }, glm::vec2{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false);
			new_hitbox(2, 1, 30, 5, 1.2, 1, glm::vec2{ -5,35 }, glm::vec2{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false);
			new_hitbox(3, 2, 30, 5, 1.2, 1, glm::vec2{ -5,35 }, glm::vec2{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false);
		}
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