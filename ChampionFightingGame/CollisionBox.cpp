#include "CollisionBox.h"
#include "BattleObject.h"
#include "Projectile.h"

CollisionBox::CollisionBox() {
	object = nullptr;
	init_anchor = glm::vec2(0.0);
	init_offset = glm::vec2(0.0);
	active = false;
}

void CollisionBox::update_pos() {
	glm::vec2 anchor = init_anchor;
	glm::vec2 offset = init_offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
}

void CollisionBox::clear() {
	active = false;
}

Pushbox::Pushbox() {
	id = -1;
}

void Pushbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(255, 255, 0, 204));
}

void Pushbox::activate(int id, glm::vec2 anchor, glm::vec2 offset) {
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
	active = true;
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

void Blockbox::activate(glm::vec2 anchor, glm::vec2 offset) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	active = true;
}

Hitbox::Hitbox() {
	id = -1;
	multihit = 0;
	collision_kind = COLLISION_KIND_GROUND;
	critical_condition = CRITICAL_CONDITION_NONE;
	hit_status = HIT_STATUS_NONE;
	custom_hit_status = 0;
	hit_flags = HIT_FLAG_NONE;
	critical_status = HIT_STATUS_NONE;
	custom_critical_status = 0;
	critical_hit_flags = HIT_FLAG_NONE;
	juggle_start = 0;
	juggle_increase = 0;
	juggle_max = 0;
	hit_height = HIT_HEIGHT_MID;
	damage = 0.0;
	chip_damage = 0.0;
	damage_scale = 0;
	meter_gain = 0.0;
	hitlag = 0;
	blocklag = 0;
	hitstun = 0;
	blockstun = 0;
	damage_kind = DAMAGE_KIND_NORMAL;
	hit_level = HIT_LEVEL_LIGHT;
	hit_effect = "";
	hit_sound = "";
}

void Hitbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(255, 0, 0, 204));
}

void Hitbox::activate(int id, int multihit, glm::vec2 anchor, glm::vec2 offset,
	CollisionKind collision_kind, float damage, float chip_damage, int damage_scale, 
	float meter_gain, int hitlag, int hitstun, int blocklag, int blockstun, HitStatus hit_status, 
	unsigned int custom_hit_status, MoveOpponent move_opponent, HitFlag hit_flags, 
	CriticalCondition critical_condition, HitStatus critical_status, 
	unsigned int custom_critical_status, MoveOpponent critical_move_opponent, 
	HitFlag critical_hit_flags, int juggle_start, int juggle_increase, int juggle_max, 
	HitHeight hit_height, DamageKind damage_kind, HitLevel hit_level, std::string hit_effect, 
	std::string hit_sound) {
	this->id = id;
	this->multihit = multihit;
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->collision_kind = collision_kind;
	this->damage = damage;
	this->chip_damage = chip_damage * damage;
	this->damage_scale = damage_scale;
	this->meter_gain = meter_gain;
	this->hitlag = hitlag;
	this->hitstun = hitstun;
	this->blocklag = blocklag;
	this->blockstun = blockstun;
	this->hit_status = hit_status;
	this->custom_hit_status = custom_hit_status;
	this->move_opponent = move_opponent;
	this->hit_flags = hit_flags;
	this->critical_condition = critical_condition;
	this->critical_status = critical_status;
	this->custom_critical_status = custom_critical_status;
	this->critical_move_opponent = critical_move_opponent;
	this->critical_hit_flags = critical_hit_flags;
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->juggle_max = juggle_max;
	this->hit_height = hit_height;
	this->damage_kind = damage_kind;
	this->hit_level = hit_level;
	this->hit_effect = hit_effect;
	this->hit_sound = hit_sound;
	this->active = true;
}

DefiniteHitbox::DefiniteHitbox() {
	attacker = nullptr;
	defender = nullptr;
	hit_status = 0;
	hit_flags = HIT_FLAG_NONE;
	juggle_start = 0;
	juggle_increase = 0;
	damage = 0.0;
	damage_scale = 0;
	meter_gain = 0.0;
	hitlag = 0;
	hitstun = 0;
	damage_kind = DAMAGE_KIND_NORMAL;
	hit_level = HIT_LEVEL_LIGHT;
	hit_effect = "";
	hit_sound = "";
	active = false;
}

void DefiniteHitbox::set_properties(BattleObject* object, Fighter* defender, Hitbox* hitbox) {
	switch (object->object_type) {
		case (BATTLE_OBJECT_TYPE_FIGHTER): {
			this->attacker = (Fighter*)object;
		} break;
		case (BATTLE_OBJECT_TYPE_PROJECTILE): {
			this->attacker = ((Projectile*)object)->owner;
		} break;
	}
	this->defender = defender;


}

void DefiniteHitbox::set_properties(BattleObject* object, Fighter* target, unsigned int hit_status, 
	HitFlag hit_flags, int juggle_start, int juggle_increase, float damage, int damage_scale, 
	float meter_gain, int hitlag, int hitstun, MoveOpponent move_opponent, DamageKind damage_kind,
	HitLevel hit_level, std::string hit_effect, std::string hit_sound) {
	switch (object->object_type) {
		case (BATTLE_OBJECT_TYPE_FIGHTER): {
			this->attacker = (Fighter*)object;
		} break;
		case (BATTLE_OBJECT_TYPE_PROJECTILE): {
			this->attacker = ((Projectile*)object)->owner;
		} break;
	}
	this->defender = target;
	this->hit_status = hit_status;
	this->hit_flags = hit_flags;
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->damage = damage;
	this->damage_scale = damage_scale;
	this->meter_gain = meter_gain;
	this->hitlag = hitlag;
	this->hitstun = hitstun;
	this->move_opponent = move_opponent;
	this->damage_kind = damage_kind;
	this->hit_level = hit_level;
	this->hit_effect = hit_effect;
	this->hit_sound = hit_sound;
}

void DefiniteHitbox::activate() {
	this->active = true;
}

void DefiniteHitbox::clear() {
	attacker = nullptr;
	defender = nullptr;
	hit_status = 0;
	hit_flags = HIT_FLAG_NONE;
	juggle_start = 0;
	juggle_increase = 0;
	damage = 0.0;
	damage_scale = 0;
	meter_gain = 0.0;
	hitlag = 0;
	hitstun = 0;
	move_opponent = MoveOpponent();
	damage_kind = DAMAGE_KIND_NORMAL;
	hit_level = HIT_LEVEL_LIGHT;
	hit_effect = "";
	hit_sound = "";
	active = false;
}

Grabbox::Grabbox() {
	id = -1;
	grabbox_kind = GRABBOX_KIND_NORMAL;
	collision_kind = COLLISION_KIND_GROUND;
	attacker_status_if_hit = 0;
	defender_status_if_hit = 0;
}

void Grabbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_alpha(204);
}

void Grabbox::activate(int id, glm::vec2 anchor, glm::vec2 offset, 
	GrabboxKind grabbox_kind, CollisionKind hit_kind, unsigned int attacker_status_if_hit,
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
	this->id = id;
	this->grabbox_kind = grabbox_kind;
	this->collision_kind = hit_kind;
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

Hurtbox::Hurtbox() {
	id = -1;
	hurtbox_kind = HURTBOX_KIND_NORMAL;
	armor_hits = 0;
	intangible_kind = INTANGIBLE_KIND_NONE;
}

void Hurtbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(0, 0, 255, 204));
}

void Hurtbox::activate(int id, glm::vec2 anchor, glm::vec2 offset, 
	HurtboxKind hurtbox_kind, int armor_hits, IntangibleKind intangible_kind) {
	anchor.x *= object->facing_dir;
	offset.x *= object->facing_dir;
	this->init_anchor = anchor;
	this->init_offset = offset;
	anchor.x += object->pos.x;
	anchor.y += object->pos.y;
	offset.x += object->pos.x;
	offset.y += object->pos.y;
	this->rect.update_corners(anchor, offset);
	this->id = id;
	this->hurtbox_kind = hurtbox_kind;
	this->armor_hits = armor_hits;
	this->intangible_kind = intangible_kind;
	this->active = true;
}