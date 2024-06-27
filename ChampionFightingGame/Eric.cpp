#include "Eric.h"
#include "EricFireball.h"
#include "ProjectileInterface.h"

Eric::Eric() {
	
}

Eric::Eric(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ERIC;
	chara_name = "eric";
	resource_dir = "resource/chara/eric";
	object_int.resize(CHARA_ERIC_INT_MAX, 0);
	object_float.resize(CHARA_ERIC_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ERIC_FLAG_MAX, false);
	object_string.resize(CHARA_ERIC_STRING_MAX, "");
	status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);

	projectiles.push_back(create_projectile(PROJECTILE_KIND_ERIC_FIREBALL, player, this));
	
	load_fighter();
}

void Eric::chara_main() {

}

void Eric::load_chara_effects() {
	load_effect("flame");
}