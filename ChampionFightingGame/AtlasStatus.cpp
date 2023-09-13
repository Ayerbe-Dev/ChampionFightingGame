#include "Atlas.h"

void Atlas::chara_main() {

}

bool Atlas::chara_ground_status_act() {
	return false;
}

bool Atlas::chara_air_status_act() {
	return false;
}

bool Atlas::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (situation_kind == FIGHTER_SITUATION_GROUND && chara_ground_status_act()) {
			return true;
		}
		else if (situation_kind == FIGHTER_SITUATION_AIR && chara_air_status_act()) {
			return true;
		}
	}
	return false;
}

void Atlas::load_chara_status_scripts() {
	status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
}