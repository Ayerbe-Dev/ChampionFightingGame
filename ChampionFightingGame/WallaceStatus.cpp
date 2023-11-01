#include "Wallace.h"

void Wallace::chara_main() {

}

bool Wallace::chara_ground_status_act() {
	return false;
}

bool Wallace::chara_air_status_act() {
	return false;
}

bool Wallace::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Wallace::load_chara_status_scripts() {
	status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
}