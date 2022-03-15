#include "Roy.h"
#include "Battle.h"
#include "RoyFireball.h"
#include "ProjectileInterface.h"

Roy::Roy() {

}

Roy::Roy(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	this->fighter_accessor = fighter_accessor;
	this->chara_kind = CHARA_KIND_ROY;
	chara_name = "roy";
	resource_dir = "resource/chara/roy";
	fighter_int.resize(CHARA_ROY_INT_MAX, 0);
	fighter_float.resize(CHARA_ROY_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ROY_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	projectiles[0] = create_projectile(PROJECTILE_KIND_ROY_FIREBALL, id, player_info, this, fighter_accessor);
}

void Roy::chara_id() {

}

void Roy::loadCharaSounds() {
	loadVC(ROY_VC_ATTACK_01);
}