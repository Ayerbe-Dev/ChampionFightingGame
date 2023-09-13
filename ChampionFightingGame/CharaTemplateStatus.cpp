#include "CharaTemplate.h"

void CharaTemplate::chara_main() {

}

bool CharaTemplate::chara_ground_status_act() {
	return false;
}

bool CharaTemplate::chara_air_status_act() {
	return false;
}

bool CharaTemplate::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void CharaTemplate::load_chara_status_scripts() {

}