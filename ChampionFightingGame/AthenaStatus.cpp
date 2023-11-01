#include "Athena.h"

void Athena::chara_main() {

}

bool Athena::chara_ground_status_act() {
	return false;
}

bool Athena::chara_air_status_act() {
	return false;
}

bool Athena::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Athena::load_chara_status_scripts() {
	status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
}