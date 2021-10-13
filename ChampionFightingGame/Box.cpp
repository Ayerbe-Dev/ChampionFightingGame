#include "Box.h"
#include "PlayerInfo.h"
#include "utils.h"

Hitbox::Hitbox() {
	this->id = -1;
}

Hitbox::Hitbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hitbox_kind, int situation_hit, int attack_level, int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable, bool success_hit, int juggle_set, int max_juggle, bool use_player_pos)
{
	if (use_player_pos) {
		anchor.x = ((anchor.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y += player_info->pos.y;
		offset.x = ((offset.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
		offset.y += player_info->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_WIDTH / 2;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y + WINDOW_HEIGHT / 2;
	this->rect.w = offset.x;
	this->rect.h = offset.y + WINDOW_HEIGHT / 2;
	this->player_info = player_info;
	this->id = id;
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

void Hitbox::clear() {
	this->id = -1;
}

Grabbox::Grabbox() { }
Grabbox::Grabbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	if (use_player_pos) {
		anchor.x = ((anchor.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y += player_info->pos.y;
		offset.x = ((offset.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
		offset.y += player_info->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_HEIGHT / 2;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y + WINDOW_HEIGHT / 2;
	this->rect.w = offset.x;
	this->rect.h = offset.y + WINDOW_HEIGHT / 2;
	this->player_info = player_info;
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->situation_hit = situation_hit;
	this->attacker_status_if_hit = attacker_status_if_hit;
}

void Grabbox::clear() {
	this->id = -1;
}

Hurtbox::Hurtbox() { }
Hurtbox::Hurtbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_invincible, bool is_armor, int intangible_kind) {
	anchor.x = ((anchor.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
	anchor.y += player_info->pos.y;
	offset.x = ((offset.x + (player_info->pos.x * player_info->facing_dir)) * player_info->facing_dir) + WINDOW_WIDTH / 2;
	offset.y += player_info->pos.y;
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y + WINDOW_HEIGHT / 2;
	this->rect.w = offset.x;
	this->rect.h = offset.y + WINDOW_HEIGHT / 2;
	this->player_info = player_info;
	this->id = id;
	this->hurtbox_kind = hurtbox_kind;
	this->is_invincible = is_invincible;
	this->is_armor = is_armor;
	this->intangible_kind = intangible_kind;
}

void Hurtbox::clear() {
	this->id = -1;
}