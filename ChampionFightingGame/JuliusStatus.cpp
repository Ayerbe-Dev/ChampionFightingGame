#include "Julius.h"

void Julius::chara_main() {

}

bool Julius::chara_ground_status_act() {
	return false;
}

bool Julius::chara_air_status_act() {
	return false;
}

bool Julius::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Julius::load_chara_status_scripts() {
	status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
}