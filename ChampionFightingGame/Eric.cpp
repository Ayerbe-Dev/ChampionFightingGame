#include "Eric.h"
#include "Battle.h"
#include "EricFireball.h"
#include "ProjectileInterface.h"

Eric::Eric() {
	
}

Eric::Eric(int id, PlayerInfo* player_info, BattleObjectManager *battle_object_manager) {
	this->player_info = player_info;
	this->battle_object_manager = battle_object_manager;
	this->chara_kind = CHARA_KIND_ERIC;
	chara_name = "eric";
	resource_dir = "resource/chara/eric";
	fighter_int.resize(CHARA_ERIC_INT_MAX, 0);
	fighter_float.resize(CHARA_ERIC_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ERIC_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadEricStatusFunctions();
	set_current_move_script("default");

	projectiles[0] = create_projectile(PROJECTILE_KIND_ERIC_FIREBALL, id, player_info, this, battle_object_manager);
}

void Eric::chara_id() {

}