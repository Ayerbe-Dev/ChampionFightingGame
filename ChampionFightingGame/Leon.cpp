#include "Leon.h"

Leon::Leon() {

}

Leon::Leon(Player* player) {
	this->id = player->id;
	this->player = player;
	if (player->chara_kind == CHARA_KIND_LEON) {
		chara_kind = CHARA_KIND_LEON;
		chara_name = "leon";
		resource_dir = "resource/chara/leon";
	}
	else {
		chara_kind = CHARA_KIND_CHAMELEON;
		chara_name = "chameleon";
		resource_dir = "resource/chara/chameleon";
	}
	object_int.resize(CHARA_LEON_INT_MAX, 0);
	object_float.resize(CHARA_LEON_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_LEON_FLAG_MAX, false);
	object_string.resize(CHARA_LEON_STRING_MAX, "");
	status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_LEON_STATUS_MAX, nullptr);

	load_fighter();
}

void Leon::chara_main() {

}