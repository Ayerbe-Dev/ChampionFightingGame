#include "Bruno.h"

Bruno::Bruno() {

}

Bruno::Bruno(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->chara_kind = CHARA_KIND_BRUNO;
	chara_name = "bruno";
	resource_dir = "resource/chara/bruno";
	fighter_int.resize(CHARA_BRUNO_INT_MAX, 0);
	fighter_float.resize(CHARA_BRUNO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_BRUNO_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadBrunoStatusFunctions();
	set_current_move_script("default");
}

void Bruno::chara_id() {

}