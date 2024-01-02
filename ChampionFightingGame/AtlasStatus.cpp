#include "Atlas.h"

bool Atlas::chara_ground_status_act() {
	return false;
}

bool Atlas::chara_status_attack() {
	return false;
}

bool Atlas::chara_status_attack_air() {
	return false;
}

void Atlas::load_chara_status_scripts() {
	status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
}