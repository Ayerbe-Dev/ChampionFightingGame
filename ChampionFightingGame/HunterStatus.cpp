#include "Hunter.h"

void Hunter::chara_main() {

}

bool Hunter::specific_ground_status_act() {
	return false;
}

bool Hunter::specific_air_status_act() {
	return false;
}

bool Hunter::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Hunter::load_chara_status_scripts() {
	status_script.resize(CHARA_HUNTER_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_HUNTER_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_HUNTER_STATUS_MAX, nullptr);
}