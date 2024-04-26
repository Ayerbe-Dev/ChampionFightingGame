#include "Julius.h"

Julius::Julius() {

}

Julius::Julius(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_JULIUS;
	chara_name = "julius";
	resource_dir = "resource/chara/julius";
	object_int.resize(CHARA_JULIUS_INT_MAX, 0);
	object_float.resize(CHARA_JULIUS_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_JULIUS_FLAG_MAX, false);
	object_string.resize(CHARA_JULIUS_STRING_MAX, "");
	status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_JULIUS_STATUS_MAX, nullptr);

	load_fighter();
}

void Julius::chara_main() {

}