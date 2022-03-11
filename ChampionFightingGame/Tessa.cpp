#include "Tessa.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Tessa::Tessa() {

}

Tessa::Tessa(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/tessa";
	chara_name = "tessa";
	fighter_int.resize(CHARA_TESSA_INT_MAX, 0);
	fighter_float.resize(CHARA_TESSA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_TESSA_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadTessaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_TESSA;
}

void Tessa::chara_id() {

}