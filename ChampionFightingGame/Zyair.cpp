#include "Zyair.h"

Zyair::Zyair() {

}

Zyair::Zyair(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ZYAIR;
	chara_name = "zyair";
	resource_dir = "resource/chara/zyair";
	fighter_int.resize(CHARA_ZYAIR_INT_MAX, 0);
	fighter_float.resize(CHARA_ZYAIR_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ZYAIR_FLAG_MAX, false);
	fighter_string.resize(CHARA_ZYAIR_STRING_MAX, "");
	load_fighter();
}

void Zyair::chara_main() {

}