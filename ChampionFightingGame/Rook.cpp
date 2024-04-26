#include "Rook.h"

Rook::Rook() {

}

Rook::Rook(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_ROOK;
	chara_name = "rook";
	resource_dir = "resource/chara/rook";
	object_int.resize(CHARA_ROOK_INT_MAX, 0);
	object_float.resize(CHARA_ROOK_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ROOK_FLAG_MAX, false);
	object_string.resize(CHARA_ROOK_STRING_MAX, "");
	status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);

	load_fighter();
}

void Rook::chara_main() {

}