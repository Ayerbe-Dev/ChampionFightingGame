#include "Bruno.h"

bool Bruno::chara_ground_status_act() {
	return false;
}

bool Bruno::chara_status_attack() {
	return false;
}

bool Bruno::chara_status_attack_air() {
	return false;
}

void Bruno::load_chara_status_scripts() {
	status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_BRUNO_STATUS_MAX, nullptr);
}