#include "CharaTemplate.h"

void CharaTemplate::chara_main() {

}

bool CharaTemplate::specific_ground_status_act() {
	return false;
}

bool CharaTemplate::specific_air_status_act() {
	return false;
}

bool CharaTemplate::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void CharaTemplate::load_chara_status_scripts() {

}