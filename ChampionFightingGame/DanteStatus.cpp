#include "Dante.h"

bool Dante::chara_ground_status_act() {
	return false;
}

bool Dante::chara_status_attack() {
	return false;
}

bool Dante::chara_status_attack_air() {
	return false;
}

void Dante::load_chara_status_scripts() {
	status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);
}