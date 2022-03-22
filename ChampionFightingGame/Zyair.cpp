#include "Zyair.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Zyair::Zyair() {

}

Zyair::Zyair(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager) {
	this->player_info = player_info;
	this->battle_object_manager = battle_object_manager;
	this->chara_kind = CHARA_KIND_ZYAIR;
	chara_name = "zyair";
	resource_dir = "resource/chara/zyair";
	fighter_int.resize(CHARA_ZYAIR_INT_MAX, 0);
	fighter_float.resize(CHARA_ZYAIR_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ZYAIR_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadZyairStatusFunctions();
	set_current_move_script("default");
}

void Zyair::chara_id() {

}