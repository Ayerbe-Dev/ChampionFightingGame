#include "Dingo.h"

Dingo::Dingo() {

}

Dingo::Dingo(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_DINGO;
	chara_name = "dingo";
	resource_dir = "resource/chara/dingo";
	fighter_int.resize(CHARA_DINGO_INT_MAX, 0);
	fighter_float.resize(CHARA_DINGO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_DINGO_FLAG_MAX, false);
	init();
}