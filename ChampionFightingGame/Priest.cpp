#include "Priest.h"

Priest::Priest() {

}

Priest::Priest(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_PRIEST;
	chara_name = "priest";
	resource_dir = "resource/chara/priest";
	fighter_int.resize(CHARA_PRIEST_INT_MAX, 0);
	fighter_float.resize(CHARA_PRIEST_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_PRIEST_FLAG_MAX, false);
	loadPriestStatusFunctions();
}

void Priest::chara_id() {

}