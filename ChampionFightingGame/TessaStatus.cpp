#include "Tessa.h"

void Tessa::chara_main() {

}

bool Tessa::chara_ground_status_act() {
	return false;
}

bool Tessa::chara_air_status_act() {
	return false;
}

bool Tessa::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Tessa::load_chara_status_scripts() {
	status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);
}