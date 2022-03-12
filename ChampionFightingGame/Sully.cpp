#include "Sully.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Sully::Sully() {

}

Sully::Sully(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	this->fighter_accessor = fighter_accessor;
	this->chara_kind = CHARA_KIND_SULLY;
	chara_name = "sully";
	resource_dir = "resource/chara/sully";
	fighter_int.resize(CHARA_SULLY_INT_MAX, 0);
	fighter_float.resize(CHARA_SULLY_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_SULLY_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadSullyStatusFunctions();
	set_current_move_script("default");
}

void Sully::chara_id() {

}