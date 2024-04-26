#include "Dingo.h"

Dingo::Dingo() {

}

Dingo::Dingo(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_DINGO;
	chara_name = "dingo";
	resource_dir = "resource/chara/dingo";
	object_int.resize(CHARA_DINGO_INT_MAX, 0);
	object_float.resize(CHARA_DINGO_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_DINGO_FLAG_MAX, false);
	object_string.resize(CHARA_DINGO_STRING_MAX, "");
	status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);

	load_fighter();
}

void Dingo::chara_main() {

}