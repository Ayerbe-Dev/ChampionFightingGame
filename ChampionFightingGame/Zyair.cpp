#include "Zyair.h"

Zyair::Zyair() {

}

Zyair::Zyair(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->chara_kind = CHARA_KIND_ZYAIR;
	chara_name = "zyair";
	resource_dir = "resource/chara/zyair";
	fighter_int.resize(CHARA_ZYAIR_INT_MAX, 0);
	fighter_float.resize(CHARA_ZYAIR_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ZYAIR_FLAG_MAX, false);
	loadZyairStatusFunctions();
}

void Zyair::chara_id() {

}