#include "Wallace.h"

Wallace::Wallace() {

}

Wallace::Wallace(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_WALLACE;
	chara_name = "wallace";
	resource_dir = "resource/chara/wallace";
	object_int.resize(CHARA_WALLACE_INT_MAX, 0);
	object_float.resize(CHARA_WALLACE_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_WALLACE_FLAG_MAX, false);
	object_string.resize(CHARA_WALLACE_STRING_MAX, "");
	status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_WALLACE_STATUS_MAX, nullptr);

	load_fighter();
}

void Wallace::chara_main() {

}