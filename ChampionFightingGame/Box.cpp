#include "Box.h"
#include "BattleObject.h"

Blockbox::Blockbox() {
	active = false;
}

void Blockbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(glm::vec4(255, 165, 0, 204));
}

void Blockbox::activate(BattleObject* object, glm::vec2 anchor, glm::vec2 offset) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->rect.bind_scale(&object->scale);
	this->object = object;
	active = true;
}

void Blockbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void Blockbox::clear() {
	active = false;
}

Hitbox::Hitbox() {
	id = -1;
	active = false;
}

void Hitbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(glm::vec4(255, 0, 0, 204));
}

void Hitbox::activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, glm::vec2 anchor,
	glm::vec2 offset, int hitbox_kind, float  meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, 
	int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
	int juggle_start, int juggle_increase, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
	float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool can_ko, bool use_player_pos) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	if (use_player_pos) {
		anchor.x += object->pos.x;
		anchor.y += object->pos.y;
		offset.x += object->pos.x;
		offset.y += object->pos.y;
	}
	this->rect.update_corners(anchor, offset);
	this->rect.bind_scale(&object->scale);
	this->object = object;
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
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->max_juggle = max_juggle;
	this->hit_status = hit_status;
	this->counterhit_status = counterhit_status;
	this->counterhit_type = counterhit_type;
	this->hit_pushback = hit_pushback * 10;
	this->block_pushback = block_pushback * 10;
	this->launch_init_y = launch_init_y;
	this->launch_gravity_y = launch_gravity_y;
	this->launch_max_fall_speed = launch_max_fall_speed;
	this->launch_speed_x = launch_speed_x;
	this->continue_launch = continue_launch;
	this->can_chip_ko = can_chip_ko;
	this->can_ko = can_ko;
	this->use_player_pos = use_player_pos;
	this->active = true;
}

/*
	The version of the Hitbox func used by projectiles
*/
void Hitbox::activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, glm::vec2 anchor,
	glm::vec2 offset, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_start, int juggle_increase, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, 
	bool continue_launch, bool can_chip_ko, bool can_ko) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	use_player_pos = true;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->rect.bind_scale(&object->scale);
	this->object = object;
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
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->max_juggle = max_juggle;
	this->hit_status = hit_status;
	this->counterhit_status = counterhit_status;
	this->counterhit_type = counterhit_type;
	this->hit_pushback = hit_pushback * 10;
	this->block_pushback = block_pushback * 10;
	this->launch_init_y = launch_init_y;
	this->launch_gravity_y = launch_gravity_y;
	this->launch_max_fall_speed = launch_max_fall_speed;
	this->launch_speed_x = launch_speed_x;
	this->trade = trade;
	this->continue_launch = continue_launch;
	this->can_chip_ko = can_chip_ko;
	this->can_ko = can_ko;
	this->active = true;
}

/*
	Updates the position of the hitboxes relative to their owner.
*/

void Hitbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	if (use_player_pos) {
		anchor.x += object->pos.x;
		anchor.y += object->pos.y;
		offset.x += object->pos.x;
		offset.y += object->pos.y;
	}
	this->rect.update_corners(anchor, offset);
}

//Clears a hitbox. TECHNICALLY never needs to be used because the constructors already set the ids to 0 but like... gross

void Hitbox::clear() {
	id = -1;
	active = false;
}

Grabbox::Grabbox() {
	id = -1;
	active = false;
}

void Grabbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_alpha(204);
}

/*
	Grabbox. Only Fighters are actually designed to call these, because if we ever wanted to make a "projectile grab", we could just
	make an unblockable projectile that tells the attacker to create a grabbox at the defender's location and handles it from there.
*/

void Grabbox::activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, int grabbox_kind, int situation_hit, unsigned int attacker_status_if_hit,
	unsigned int defender_status_if_hit, bool use_player_pos) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	if (use_player_pos) {
		anchor.x += object->pos.x;
		anchor.y += object->pos.y;
		offset.x += object->pos.x;
		offset.y += object->pos.y;
	}
	this->rect.update_corners(anchor, offset);
	this->rect.bind_scale(&object->scale);
	this->object = object;
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->situation_hit = situation_hit;
	this->attacker_status_if_hit = attacker_status_if_hit;
	this->defender_status_if_hit = defender_status_if_hit;
	this->use_player_pos = use_player_pos;
	if (grabbox_kind & GRABBOX_KIND_NOTECH) {
		rect.set_rgb(glm::vec3(128, 0, 128));
	}
	else {
		rect.set_rgb(glm::vec3(0, 255, 0));
	}
	this->active = true;
}

void Grabbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	if (use_player_pos) {
		anchor.x += object->pos.x;
		anchor.y += object->pos.y;
		offset.x += object->pos.x;
		offset.y += object->pos.y;
	}
	this->rect.update_corners(anchor, offset);
}

void Grabbox::clear() {
	id = -1;
	active = false;
}

Hurtbox::Hurtbox() {
	id = -1;
	active = false;
}

void Hurtbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(glm::vec4(0, 0, 255, 204));
}

/*
	Hurtbox. Only used by Fighters since projectile trades work by comparing the hitboxes directly. Also I think it's safe to assume that
	Hurtboxes will never use a global position
*/

void Hurtbox::activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, int hurtbox_kind, bool is_armor, int intangible_kind) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->rect.bind_scale(&object->scale);
	this->object = object;
	this->id = id;
	this->hurtbox_kind = hurtbox_kind;
	this->is_armor = is_armor;
	this->intangible_kind = intangible_kind;
	this->active = true;
}

void Hurtbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void Hurtbox::clear() {
	id = -1;
	active = false;
}