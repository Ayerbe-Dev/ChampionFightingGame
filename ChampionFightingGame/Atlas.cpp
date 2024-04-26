#include "Atlas.h"

Atlas::Atlas() {

}

Atlas::Atlas(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ATLAS;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
	object_int.resize(CHARA_ATLAS_INT_MAX, 0);
	object_float.resize(CHARA_ATLAS_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ATLAS_FLAG_MAX, false);
	object_string.resize(CHARA_ATLAS_STRING_MAX, "");
	status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATLAS_STATUS_MAX, nullptr);

	load_fighter();
}

void Atlas::chara_main() {

}