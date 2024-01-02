#include "Wallace.h"

bool Wallace::chara_ground_status_act() {
	return false;
}

bool Wallace::chara_status_attack() {
	return false;
}

bool Wallace::chara_status_attack_air() {
	return false;
}

void Wallace::load_chara_status_scripts() {
	status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
}