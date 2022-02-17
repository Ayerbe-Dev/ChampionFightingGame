#include "Box.h"
#include "BattleObject.h"
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

void Hitbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(vec4(255, 0, 0, 127));
}

void Hitbox::activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, vec2 anchor,
	vec2 offset, int hitbox_kind, float  meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, 
	int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind,
	int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
	float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool use_player_pos) {
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
	this->can_chip_ko = can_chip_ko;
	this->use_player_pos = use_player_pos;
}

/*
	The version of the Hitbox func used by projectiles
*/
void Hitbox::activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, vec2 anchor,
	vec2 offset, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
	int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status,
	int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, 
	bool continue_launch, bool can_chip_ko) {
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
	this->can_chip_ko = can_chip_ko;
	this->continue_launch = continue_launch;
}

/*
	Updates the position of the hitboxes relative to their owner.Projectiles already include the window width to calculate their location, so
	they shouldn't add it a second time, hence the second arg.
*/

void Hitbox::update_pos(BattleObject * object, bool add_window_width) {
	vec2 anchor = init_anchor;
	vec2 offset = init_offset;
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
	if (id != -1) {
		this->id = -1;
	}
}

Grabbox::Grabbox() {
	this->id = -1;
}

void Grabbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(vec4(0, 255, 0, 127));
}

/*
	Grabbox. Only Fighters are actually designed to call these, because if we ever wanted to make a "projectile grab", we could just
	make an unblockable projectile that tells the attacker to create a grabbox at the defender's location and handles it from there.
*/

void Grabbox::activate(BattleObject* object, int id, vec2 anchor, vec2 offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
	u32 defender_status_if_hit, bool use_player_pos) {
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
}

void Grabbox::update_pos(BattleObject* object) {
	vec2 anchor = init_anchor;
	vec2 offset = init_offset;
	if (use_player_pos) {
		anchor.x += object->pos.x;
		anchor.y += object->pos.y;
		offset.x += object->pos.x;
		offset.y += object->pos.y;
	}
	this->rect.update_corners(anchor, offset);
}

void Grabbox::clear() {
	this->id = -1;
}

Hurtbox::Hurtbox() {
	this->id = -1;
}

void Hurtbox::init(BattleObject* object) {
	rect.init();
	rect.bind_scale(&object->scale);
	rect.set_rgba(vec4(0, 0, 255, 127));
}

/*
	Hurtbox. Only used by Fighters since projectile trades work by comparing the hitboxes directly. Also I think it's safe to assume that
	Hurtboxes will never use a global position
*/

void Hurtbox::activate(BattleObject* object, int id, vec2 anchor, vec2 offset, int hurtbox_kind, bool is_armor, int intangible_kind) {
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
}

void Hurtbox::update_pos(BattleObject *object) {
	vec2 anchor = init_anchor;
	vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void Hurtbox::clear() {
	this->id = -1;
}