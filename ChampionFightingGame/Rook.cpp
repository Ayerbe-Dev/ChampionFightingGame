#include "Rook.h"

Rook::Rook() {

}

Rook::Rook(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_ROOK;
	chara_name = "rook";
	resource_dir = "resource/chara/rook";
	fighter_int.resize(CHARA_ROOK_INT_MAX, 0);
	fighter_float.resize(CHARA_ROOK_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ROOK_FLAG_MAX, false);
	load_fighter();
}