#include "Priest.h"

Priest::Priest() {

}

Priest::Priest(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->chara_kind = CHARA_KIND_PRIEST;
	chara_name = "priest";
	resource_dir = "resource/chara/priest";
	fighter_int.resize(CHARA_PRIEST_INT_MAX, 0);
	fighter_float.resize(CHARA_PRIEST_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_PRIEST_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadPriestStatusFunctions();
	set_current_move_script("default");
}

void Priest::chara_id() {

}