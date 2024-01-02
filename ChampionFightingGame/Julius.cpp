#include "Julius.h"

Julius::Julius() {

}

Julius::Julius(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_JULIUS;
	chara_name = "julius";
	resource_dir = "resource/chara/julius";
	fighter_int.resize(CHARA_JULIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_JULIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_JULIUS_FLAG_MAX, false);
	fighter_string.resize(CHARA_JULIUS_STRING_MAX, "");
	load_fighter();
}

void Julius::chara_main() {

}