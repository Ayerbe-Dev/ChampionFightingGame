#include "Norman.h"

Norman::Norman() {

}

Norman::Norman(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager) {
	this->player_info = player_info;
	this->battle_object_manager = battle_object_manager;
	this->chara_kind = CHARA_KIND_NORMAN;
	chara_name = "norman";
	resource_dir = "resource/chara/norman";
	fighter_int.resize(CHARA_NORMAN_INT_MAX, 0);
	fighter_float.resize(CHARA_NORMAN_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NORMAN_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadNormanStatusFunctions();
	set_current_move_script("default");
}

void Norman::chara_id() {

}