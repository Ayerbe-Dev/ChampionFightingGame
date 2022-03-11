#include "Bruno.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Bruno::Bruno() {

}

Bruno::Bruno(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/bruno";
	chara_name = "bruno";
	fighter_int.resize(CHARA_BRUNO_INT_MAX, 0);
	fighter_float.resize(CHARA_BRUNO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_BRUNO_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadBrunoStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_BRUNO;
}

void Bruno::chara_id() {

}