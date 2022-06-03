#include "Eric.h"
#include "EricFireball.h"
#include "ProjectileInterface.h"

Eric::Eric() {
	
}

Eric::Eric(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_ERIC;
	chara_name = "eric";
	resource_dir = "resource/chara/eric";
	fighter_int.resize(CHARA_ERIC_INT_MAX, 0);
	fighter_float.resize(CHARA_ERIC_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ERIC_FLAG_MAX, false);
	loadEricStatusFunctions();
	projectiles[0] = create_projectile(PROJECTILE_KIND_ERIC_FIREBALL, id, player, this);
}

void Eric::chara_id() {

}