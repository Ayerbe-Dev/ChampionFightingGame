#include "Roy.h"
#include "RoyFireball.h"
#include "ProjectileInterface.h"
#include "SoundConstants.h"

Roy::Roy() {

}

Roy::Roy(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_ROY;
	chara_name = "roy";
	resource_dir = "resource/chara/roy";
	fighter_int.resize(CHARA_ROY_INT_MAX, 0); 
	fighter_float.resize(CHARA_ROY_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ROY_FLAG_MAX, false);
	loadRoyStatusFunctions();
	projectiles[0] = create_projectile(PROJECTILE_KIND_ROY_FIREBALL, id, player, this);
}

void Roy::chara_id() {

}

void Roy::load_chara_sounds() {
	load_sound("roy_attack_01");
}

void Roy::load_chara_effects() {
	load_effect("flame");
}