#include "Tessa.h"

Tessa::Tessa() {

}

Tessa::Tessa(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_TESSA;
	chara_name = "tessa";
	resource_dir = "resource/chara/tessa";
	object_int.resize(CHARA_TESSA_INT_MAX, 0);
	object_float.resize(CHARA_TESSA_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_TESSA_FLAG_MAX, false);
	object_string.resize(CHARA_TESSA_STRING_MAX, "");
	status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_TESSA_STATUS_MAX, nullptr);

	load_fighter();
}

void Tessa::chara_main() {

}