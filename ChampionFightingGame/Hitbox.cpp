using namespace std;
#include "Hitbox.h"
#include "PlayerInfo.h"
#include "utils.h"

Hitbox::Hitbox() { }

Hitbox::Hitbox(PlayerInfo *player_info, int id, int x0, int x1, int y0, int y1, bool raw_coords, int hitbox_kind, int situation_hit, int attack_level,
	int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable, bool success_hit, 
	int juggle_set, int max_juggle) {

	this->player_info = player_info;
	this->id = id;
	if (raw_coords) {
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
	}
	else {
		this->x0 = (*player_info).pos.x + x0;
		this->y0 = (*player_info).pos.y + y0;
		this->x1 = this->x0 + x1;
		this->y1 = this->y0 + y1;
	}
	this->hitbox_kind = hitbox_kind;
	this->situation_hit = situation_hit;
	this->attack_level = attack_level;
	this->clank_kind = clank_kind;
	this->damage = damage;
	this->hitlag = hitlag;
	this->hitstun = hitstun;
	this->blocklag = blocklag;
	this->blockstun = blockstun;
	this->attack_height = attack_height;
	this->meter_gain = meter_gain;
	this->unblockable = unblockable;
	this->success_hit = success_hit;
	this->juggle_set = juggle_set;
	this->max_juggle = max_juggle;
}

Grabbox::Grabbox() { }

Grabbox::Grabbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, bool raw_coords, int grabbox_kind, int situation_hit, 
	u32 attacker_status_if_hit, u32 defender_status_if_hit) {

	this->player_info = player_info;
	this->id = id;
	if (raw_coords) {
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
	}
	else {
		this->x0 = (*player_info).pos.x + x0;
		this->y0 = (*player_info).pos.y + y0;
		this->x1 = this->x0 + x1;
		this->y1 = this->y0 + y1;
	}
	this->grabbox_kind = grabbox_kind;
	this->situation_hit = situation_hit;
	this->attacker_status_if_hit = attacker_status_if_hit;
}

Hurtbox::Hurtbox() { }

Hurtbox::Hurtbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, int hurtbox_kind, bool is_invincible, bool is_armor, int intangible_kind) {
	this->player_info = player_info;
	this->id = id;
	this->x0 = (*player_info).pos.x + x0;
	this->y0 = (*player_info).pos.y + y0;
	this->x1 = this->x0 + x1;
	this->y1 = this->y0 + y1;
	this->hurtbox_kind = hurtbox_kind;
	this->is_invincible = is_invincible;
	this->is_armor = is_armor;
	this->intangible_kind = intangible_kind;
}