#include "Zyair.h"

Zyair::Zyair() {

}

Zyair::Zyair(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ZYAIR;
	chara_name = "zyair";
	resource_dir = "resource/chara/zyair";
	object_int.resize(CHARA_ZYAIR_INT_MAX, 0);
	object_float.resize(CHARA_ZYAIR_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ZYAIR_FLAG_MAX, false);
	object_string.resize(CHARA_ZYAIR_STRING_MAX, "");
	status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);

	load_fighter();
}

void Zyair::chara_main() {

}