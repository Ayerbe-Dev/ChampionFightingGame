#include "Bruno.h"

void Bruno::chara_main() {

}

bool Bruno::chara_ground_status_act() {
	return false;
}

bool Bruno::chara_air_status_act() {
	return false;
}

bool Bruno::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Bruno::load_chara_status_scripts() {
	status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
}