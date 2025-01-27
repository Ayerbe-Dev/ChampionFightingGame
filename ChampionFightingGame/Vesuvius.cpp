#include "Vesuvius.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_VESUVIUS;
	chara_name = "vesuvius";
	resource_dir = "resource/chara/vesuvius";
	object_int.resize(CHARA_VESUVIUS_INT_MAX, 0);
	object_float.resize(CHARA_VESUVIUS_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_VESUVIUS_FLAG_MAX, false);
	object_string.resize(CHARA_VESUVIUS_STRING_MAX, "");
	status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);

	load_fighter();
}

void Vesuvius::chara_main() {

}