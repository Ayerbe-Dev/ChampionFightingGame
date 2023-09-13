#include "Box.h"
#include "BattleObject.h"

Pushbox::Pushbox() {
	id = -1;
	object = nullptr;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	active = false;
}

void Pushbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(255, 255, 0, 204));
}

void Pushbox::activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset) {
	this->id = id;
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->object = object;
	active = true;
}

void Pushbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void Pushbox::clear() {
	active = false;
}

Blockbox::Blockbox() {
	object = nullptr;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	active = false;
}

void Blockbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
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
	object = nullptr;
	id = -1;
	multihit = 0;
	damage = 0.0;
	chip_damage = 0.0;
	damage_scale = 0;
	meter_gain = 0.0;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	hit_kind = HIT_KIND_GROUND;
	attack_level = ATTACK_LEVEL_LIGHT;
	attack_height = ATTACK_HEIGHT_MID;
	hitlag = 0;
	blocklag = 0;
	hitstun = 0;
	blockstun = 0;
	hit_pushback = 0.0;
	block_pushback = 0.0;
	hit_status = HIT_STATUS_NORMAL;
	counterhit_status = HIT_STATUS_NORMAL;
	counterhit_type = COUNTERHIT_TYPE_COUNTER;
	juggle_start = 0;
	juggle_increase = 0;
	juggle_max = 0;
	clank_kind = CLANK_KIND_NORMAL;
	damage_kind = DAMAGE_KIND_NORMAL;
	continue_launch = false;
	disable_hitstun_parry = false;
	launch_init_y = 0.0;
	launch_gravity_y = 0.0;
	launch_max_fall_speed = 0.0;
	launch_speed_x = 0.0;
	active = false;
}

void Hitbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(255, 0, 0, 204));
}

void Hitbox::activate(BattleObject* object, int id, int multihit, float damage, float chip_damage,
	int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, HitKind hit_kind,
	AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
	int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
	HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
	int juggle_max, ClankKind clank_kind, DamageKind damage_kind, bool continue_launch,
	bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
	float launch_max_fall_speed, float launch_speed_x) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->object = object;
	this->id = id;
	this->multihit = multihit;
	this->damage = damage;
	this->chip_damage = chip_damage;
	this->damage_scale = damage_scale;
	this->meter_gain = meter_gain;
	this->hit_kind = hit_kind;
	this->attack_level = attack_level;
	this->attack_height = attack_height;
	this->hitlag = hitlag;
	this->blocklag = blocklag;
	this->hitstun = hitstun;
	this->blockstun = blockstun;
	this->hit_pushback = hit_pushback * 10.0f;
	this->block_pushback = block_pushback * 10.0f;
	this->hit_status = hit_status;
	this->counterhit_status = counterhit_status;
	this->counterhit_type = counterhit_type;
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->juggle_max = juggle_max;
	this->clank_kind = clank_kind;
	this->damage_kind = damage_kind;
	this->continue_launch = continue_launch;
	this->disable_hitstun_parry = disable_hitstun_parry;
	this->launch_init_y = launch_init_y;
	this->launch_gravity_y = launch_gravity_y;
	this->launch_max_fall_speed = launch_max_fall_speed;
	this->launch_speed_x = launch_speed_x;
	this->active = true;
}

/*
	Updates the position of the hitboxes relative to their owner.
*/

void Hitbox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

//Clears a hitbox. TECHNICALLY never needs to be used because the constructors already set the ids to 0 but like... gross

void Hitbox::clear() {
	id = -1;
	active = false;
}

Grabbox::Grabbox() {
	object = nullptr;
	id = -1;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	grabbox_kind = GRABBOX_KIND_NORMAL;
	hit_kind = HIT_KIND_GROUND;
	attacker_status_if_hit = 0;
	defender_status_if_hit = 0;
	active = false;
}

void Grabbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_alpha(204);
}

/*
	Grabbox. Only Fighters are actually designed to call these, because if we ever wanted to make a "projectile grab", we could just
	make an unblockable projectile that tells the attacker to create a grabbox at the defender's location and handles it from there.
*/

void Grabbox::activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, 
	GrabboxKind grabbox_kind, HitKind hit_kind, unsigned int attacker_status_if_hit,
	unsigned int defender_status_if_hit) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->object = object;
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->hit_kind = hit_kind;
	this->attacker_status_if_hit = attacker_status_if_hit;
	this->defender_status_if_hit = defender_status_if_hit;
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
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void Grabbox::clear() {
	id = -1;
	active = false;
}

Hurtbox::Hurtbox() {
	object = nullptr;
	id = -1;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	hurtbox_kind = HURTBOX_KIND_NORMAL;
	is_armor = false;
	intangible_kind = INTANGIBLE_KIND_NONE;
	active = false;
}

void Hurtbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(0, 0, 255, 204));
}

/*
	Hurtbox. Only used by Fighters since projectile trades work by comparing the hitboxes directly. Also I think it's safe to assume that
	Hurtboxes will never use a global position
*/

void Hurtbox::activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, 
	HurtboxKind hurtbox_kind, bool is_armor, IntangibleKind intangible_kind) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
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