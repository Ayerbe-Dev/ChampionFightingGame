#include "Julius.h"

Julius::Julius() {

}

Julius::Julius(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_JULIUS;
	chara_name = "julius";
	resource_dir = "resource/chara/julius";
	fighter_int.resize(CHARA_JULIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_JULIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_JULIUS_FLAG_MAX, false);
	loadJuliusStatusFunctions();
}

void Julius::chara_id() {

}