#include "Desirae.h"

Desirae::Desirae() {

}

Desirae::Desirae(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_DESIRAE;
	chara_name = "desirae";
	resource_dir = "resource/chara/desirae";
	object_int.resize(CHARA_DESIRAE_INT_MAX, 0);
	object_float.resize(CHARA_DESIRAE_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_DESIRAE_FLAG_MAX, false);
	object_string.resize(CHARA_DESIRAE_STRING_MAX, "");
	status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);

	load_fighter();
}

void Desirae::chara_main() {

}