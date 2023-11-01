#include "Zyair.h"

void Zyair::chara_main() {

}

bool Zyair::chara_ground_status_act() {
	return false;
}

bool Zyair::chara_air_status_act() {
	return false;
}

bool Zyair::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Zyair::load_chara_status_scripts() {
	status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
}