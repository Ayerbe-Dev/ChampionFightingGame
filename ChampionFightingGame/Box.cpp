#include "Box.h"
#include "ObjectInstance.h"
#include "utils.h"

Hitbox::Hitbox() {
	this->id = -1;
}

/*
	All of the functions related to Hitboxes, Grabboxesand Hurtboxes. Note: None of these functions are ever called directly, the Fighter and 
	Projectile Instances all contain functions that call these.
*/

/*
	The version of the Hitbox func used by players
*/

Hitbox::Hitbox(ObjectInstance* object_instance, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
	GameCoordinate offset, int hitbox_kind, float  meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, 
	int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
	bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
	float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool use_player_pos) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	if (use_player_pos) {
		anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * - 1.0 - object_instance->pos.y;
		offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0;
		offset.x += WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->object_instance = object_instance;
	this->id = id;
	this->multihit = multihit;
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
	this->continue_launch = continue_launch;
	this->use_player_pos = use_player_pos;
}

/*
	The version of the Hitbox func used by projectiles
*/
Hitbox::Hitbox(ObjectInstance* object_instance, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
	GameCoordinate offset, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, bool success_hit, int juggle_set, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, bool continue_launch) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir);
	anchor.y = anchor.y + object_instance->pos.y;
	offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir);
	offset.y = offset.y + object_instance->pos.y;
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->object_instance = object_instance;
	this->id = id;
	this->multihit = multihit;
	this->situation_hit = situation_hit;
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
	this->trade = trade;
	this->continue_launch = continue_launch;
}

/*
	Updates whether or not a hitbox has already connected based on its multihit index
*/

void Hitbox::update_connect(int multihit_index) {
	if (multihit == multihit_index) {
		this->success_hit = true;
	}
}

/*
	Updates the position of the hitboxes relative to their owner.Projectiles already include the window width to calculate their location, so
	they shouldn't add it a second time, hence the second arg.
*/

void Hitbox::update_pos(ObjectInstance * object_instance, bool add_window_width) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	if (this->use_player_pos) {
		if (add_window_width) {
			anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
			anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
			offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
			offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
		}
		else {
			anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir);
			anchor.y = anchor.y + object_instance->pos.y;
			offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir);
			offset.y = offset.y + object_instance->pos.y;
		}
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0;
		offset.x += WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0;
	}

	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
}

//Clears a hitbox. TECHNICALLY never needs to be used because the constructors already set the ids to 0 but like... gross

void Hitbox::clear() {
	this->id = -1;
}

Grabbox::Grabbox() {
	this->id = -1;
}

/*
	Grabbox. Only FighterInstances are actually designed to call these, because if we ever wanted to make a "projectile grab", we could just
	make an unblockable projectile that tells the attacker to create a grabbox at the defender's location and handles it from there.
*/

Grabbox::Grabbox(ObjectInstance* object_instance, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	if (use_player_pos) {
		anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
		offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0;
		offset.x += WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0;
	}
	offset.x -= anchor.x;
	offset.y -= anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->object_instance = object_instance;
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->situation_hit = situation_hit;
	this->attacker_status_if_hit = attacker_status_if_hit;
	this->defender_status_if_hit = defender_status_if_hit;
	this->use_player_pos = use_player_pos;
}

void Grabbox::update_pos(ObjectInstance* object_instance) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	if (this->use_player_pos) {
		anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
		offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	}
	else {
		anchor.x += WINDOW_WIDTH / 2;
		anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0;
		offset.x += WINDOW_WIDTH / 2;
		offset.y = (offset.y - WINDOW_HEIGHT) * -1.0;
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

/*
	Hurtbox. Only used by FighterInstances since projectile trades work by comparing the hitboxes directly. Also I think it's safe to assume that
	Hurtboxes will never use a global position
*/

Hurtbox::Hurtbox(ObjectInstance* object_instance, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_armor, int intangible_kind) {
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	offset.x -= anchor.x;
	offset.y += anchor.y;
	this->rect.x = anchor.x;
	this->rect.y = anchor.y;
	this->rect.w = offset.x;
	this->rect.h = offset.y;
	this->object_instance = object_instance;
	this->id = id;
	this->hurtbox_kind = hurtbox_kind;
	this->is_armor = is_armor;
	this->intangible_kind = intangible_kind;
}

void Hurtbox::update_pos(ObjectInstance *object_instance) {
	GameCoordinate anchor;
	GameCoordinate offset;
	anchor = init_anchor;
	offset = init_offset;
	anchor.x = ((anchor.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
	anchor.y = (anchor.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
	offset.x = ((offset.x + (object_instance->pos.x * object_instance->facing_dir)) * object_instance->facing_dir) + WINDOW_WIDTH / 2;
	offset.y = (offset.y - WINDOW_HEIGHT) * -1.0 - object_instance->pos.y;
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