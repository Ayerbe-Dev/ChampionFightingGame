#include "Rowan.h"
#include "RowanFireball.h"
#include "ProjectileInterface.h"
#include "SoundConstants.h"

Rowan::Rowan() {

}

Rowan::Rowan(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_ROWAN;
	chara_name = "rowan";
	resource_dir = "resource/chara/rowan";
	fighter_int.resize(CHARA_ROWAN_INT_MAX, 0); 
	fighter_float.resize(CHARA_ROWAN_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ROWAN_FLAG_MAX, false);
	loadRoyStatusFunctions();
	projectiles[0] = create_projectile(PROJECTILE_KIND_ROWAN_FIREBALL, id, player, this);
}

void Rowan::chara_id() {

}

void Rowan::load_chara_sounds() {
	load_sound("rowan_attack_01");
}

void Rowan::load_chara_effects() {
	load_effect("flame");
}