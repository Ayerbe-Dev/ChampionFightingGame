#include "Box.h"
#include "FighterInstance.h"
#include "utils.h"

Hitbox::Hitbox() {
	this->id = -1;
}

Hitbox::Hitbox(FighterInstance* fighter_instance, int id, f32 damage, f32 chip_damage, f32 counterhit_damage_mul, int scale, GameCoordinate anchor,
	GameCoordinate offset, int hitbox_kind, f32  meter_gain_on_hit, f32 meter_gain_on_counterhit, f32 meter_gain_on_block, int situation_hit, int hitlag, 
	int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, f32 hit_pushback, f32 block_pushback, int clank_kind,
	bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, f32 launch_init_y,
	f32 launch_gravity_y, f32 launch_max_fall_speed, f32 launch_speed_x, bool use_player_pos) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	if (use_player_pos) {
		anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * - 1.0 - fighter_instance->pos.y;
		offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_WIDTH / 2;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->fighter_instance = fighter_instance;
	this->id = id;
	this->hitbox_kind = hitbox_kind;
	this->situation_hit = situation_hit;
	this->attack_level = attack_level;
	this->clank_kind = clank_kind;
	this->damage = damage;
	this->chip_damage = chip_damage;
	this->counterhit_damage_mul = counterhit_damage_mul;
	this->scale = scale;
	this->meter_gain_on_hit = meter_gain_on_hit;
	this->meter_gain_on_counterhit = meter_gain_on_counterhit;
	this->meter_gain_on_block = meter_gain_on_block;
	this->hitlag = hitlag;
	this->hitstun = hitstun;
	this->blocklag = blocklag;
	this->blockstun = blockstun;
	this->attack_height = attack_height;
	this->unblockable = unblockable;
	this->success_hit = success_hit;
	this->juggle_set = juggle_set;
	this->max_juggle = max_juggle;
	this->hit_status = hit_status;
	this->counterhit_status = counterhit_status;
	this->counterhit_type = counterhit_type;
	this->hit_pushback = hit_pushback;
	this->block_pushback = block_pushback;
	this->launch_init_y = launch_init_y;
	this->launch_gravity_y = launch_gravity_y;
	this->launch_max_fall_speed = launch_max_fall_speed;
	this->launch_speed_x = launch_speed_x;
	this->use_player_pos = use_player_pos;
}

void Hitbox::update_connect() {
	this->success_hit = true;
}

void Hitbox::update_pos(FighterInstance * fighter_instance) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	if (this->use_player_pos) {
		anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
		offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_HEIGHT / 2;
	}

	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
}

void Hitbox::clear() {
	this->id = -1;
}

Grabbox::Grabbox() {
	this->id = -1;
}

Grabbox::Grabbox(FighterInstance* fighter_instance, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	anchor = init_anchor;
	offset = init_offset;
	if (this->use_player_pos) {
		anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
		offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_HEIGHT / 2;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->fighter_instance = fighter_instance;
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->situation_hit = situation_hit;
	this->attacker_status_if_hit = attacker_status_if_hit;
	this->use_player_pos = use_player_pos;
}

void Grabbox::update_pos(FighterInstance* fighter_instance) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	if (this->use_player_pos) {
		anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
		offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		offset.x += WINDOW_HEIGHT / 2;
	}

	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
}

void Grabbox::clear() {
	this->id = -1;
}

Hurtbox::Hurtbox() {
	this->id = -1;
}

Hurtbox::Hurtbox(FighterInstance* fighter_instance, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_armor, int intangible_kind) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	offset.x -= anchor.x;
	offset.y += anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->fighter_instance = fighter_instance;
	this->id = id;
	this->hurtbox_kind = hurtbox_kind;
	this->is_armor = is_armor;
	this->intangible_kind = intangible_kind;
}

void Hurtbox::update_pos(FighterInstance *fighter_instance) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	anchor.x = ((anchor.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	offset.x = ((offset.x + (fighter_instance->pos.x * fighter_instance->facing_dir)) * fighter_instance->facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - fighter_instance->pos.y;
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
}

void Hurtbox::clear() {
	this->id = -1;
}