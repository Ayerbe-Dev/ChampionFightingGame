#include "Julius.h"

bool Julius::chara_ground_status_act() {
	return false;
}

bool Julius::chara_status_attack() {
	return false;
}

bool Julius::chara_status_attack_air() {
	return false;
}

void Julius::load_chara_status_scripts() {
	status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
}