#include "Vesuvius.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager) {
	this->player_info = player_info;
	this->battle_object_manager = battle_object_manager;
	this->chara_kind = CHARA_KIND_VESUVIUS;
	chara_name = "vesuvius";
	resource_dir = "resource/chara/vesuvius";
	fighter_int.resize(CHARA_VESUVIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_VESUVIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_VESUVIUS_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadVesuviusStatusFunctions();
	set_current_move_script("default");
}

void Vesuvius::chara_id() {

}