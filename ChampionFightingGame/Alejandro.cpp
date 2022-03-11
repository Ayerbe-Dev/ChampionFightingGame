#include "Alejandro.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Alejandro::Alejandro() {

}

Alejandro::Alejandro(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/alejandro";
	chara_name = "alejandro";
	fighter_int.resize(CHARA_ALEJANDRO_INT_MAX, 0);
	fighter_float.resize(CHARA_ALEJANDRO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ALEJANDRO_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAlejandroStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ALEJANDRO;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}
}

void Alejandro::chara_id() {

}