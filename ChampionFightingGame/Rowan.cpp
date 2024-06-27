#include "Rowan.h"
#include "RowanFireball.h"
#include "ProjectileInterface.h"
#include "SoundConstants.h"

Rowan::Rowan() {

}

Rowan::Rowan(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ROWAN;
	chara_name = "rowan";
	resource_dir = "resource/chara/rowan";
	object_int.resize(CHARA_ROWAN_INT_MAX, 0);
	object_float.resize(CHARA_ROWAN_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_ROWAN_FLAG_MAX, false);
	object_string.resize(CHARA_ROWAN_STRING_MAX, "");
	status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);

	projectiles.push_back(create_projectile(PROJECTILE_KIND_ROWAN_FIREBALL, player, this));

	load_fighter();
}

void Rowan::chara_main() {

}

void Rowan::load_chara_effects() {

}