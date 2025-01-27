#include "Athena.h"

Athena::Athena() {

}

Athena::Athena(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ATHENA;
	chara_name = "athena";
	resource_dir = "resource/chara/athena";
	object_int.resize(CHARA_ATHENA_INT_MAX, 0);
	object_float.resize(CHARA_ATHENA_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ATHENA_FLAG_MAX, false);
	object_string.resize(CHARA_ATHENA_STRING_MAX, "");
	status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);

	load_fighter();
}

void Athena::chara_main() {

}