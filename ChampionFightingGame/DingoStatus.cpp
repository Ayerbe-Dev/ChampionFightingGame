#include "Dingo.h"

void Dingo::chara_main() {

}

bool Dingo::chara_ground_status_act() {
	return false;
}

bool Dingo::chara_air_status_act() {
	return false;
}

bool Dingo::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Dingo::load_chara_status_scripts() {
	status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
}