#include "Dante.h"

Dante::Dante() {

}

Dante::Dante(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_DANTE;
	chara_name = "dante";
	resource_dir = "resource/chara/dante";
	object_int.resize(CHARA_DANTE_INT_MAX, 0);
	object_float.resize(CHARA_DANTE_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_DANTE_FLAG_MAX, false);
	object_string.resize(CHARA_DANTE_STRING_MAX, "");
	status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DANTE_STATUS_MAX, nullptr);

	load_fighter();
}

void Dante::chara_main() {

}