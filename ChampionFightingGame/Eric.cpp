#include "Eric.h"
#include "utils.h"
#include "Battle.h"
#include "EricFireball.h"
#include "ProjectileInterface.h"

Eric::Eric() {
	
}

Eric::Eric(int id, PlayerInfo* player_info, FighterAccessor *fighter_accessor) {
	resource_dir = "resource/chara/eric";
	chara_name = "eric";
	fighter_int.resize(CHARA_ERIC_INT_MAX, 0);
	fighter_float.resize(CHARA_ERIC_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ERIC_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadEricStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ERIC;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

	projectiles[0] = create_projectile(PROJECTILE_KIND_ERIC_FIREBALL, id, player_info, this, fighter_accessor);
}

void Eric::chara_id() {

}