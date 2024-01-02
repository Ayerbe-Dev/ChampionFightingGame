#include "Leon.h"

bool Leon::chara_ground_status_act() {
	return false;
}

bool Leon::chara_status_attack() {
	return false;
}

bool Leon::chara_status_attack_air() {
	return false;
}

void Leon::load_chara_status_scripts() {
	status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);
}