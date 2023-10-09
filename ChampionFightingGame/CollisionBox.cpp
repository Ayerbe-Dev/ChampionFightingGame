#include "CollisionBox.h"
#include "BattleObject.h"

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
	counterhit_type = COUNTERHIT_TYPE_COUNTER;
	hit_status = HIT_STATUS_NONE;
	custom_hit_status = 0;
	counterhit_status = HIT_STATUS_NONE;
	custom_counterhit_status = 0;
	down_face_down = false;
	continue_launch = false;
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
	disable_hitstun_parry = false;
	pushback_ground_hit = 0.0;
	pushback_ground_block = 0.0;
	pushback_air_x = 0.0;
	pushback_air_y = 0.0;
	pushback_frames = 0;
	launch_init_y = 0.0;
	launch_gravity_y = 0.0;
	launch_max_fall_speed = 0.0;
	launch_speed_x = 0.0;
	launch_target_pos = glm::vec3(0.0);
	has_launch_target_pos = false;
	damage_kind = DAMAGE_KIND_NORMAL;
	hit_level = HIT_LEVEL_LIGHT;
	hit_effect_id = -1;
	hit_sound_id = -1;
}

void Hitbox::init(BattleObject* object) {
	this->object = object;
	rect.init();
	rect.set_rgba(glm::vec4(255, 0, 0, 204));
}

void Hitbox::activate(int id, int multihit, glm::vec2 anchor, glm::vec2 offset, CollisionKind collision_kind,
	CounterhitType counterhit_type, HitStatus hit_status, unsigned int custom_hit_status,
	HitStatus counterhit_status, unsigned int custom_counterhit_status, bool knockdown_face_down,
	bool continue_launch, int juggle_start, int juggle_increase, int juggle_max, HitHeight hit_height,
	float damage, float chip_damage, int damage_scale, float meter_gain, int hitlag, int blocklag,
	int hitstun, int blockstun, bool disable_hitstun_parry, float pushback_ground_hit,
	float pushback_ground_block, float pushback_air_x, float pushback_air_y, int pushback_frames,
	float launch_init_y, float launch_gravity, float launch_max_fall_speed, float launch_speed_x,
	glm::vec3 launch_target_pos, bool has_launch_target_pos, DamageKind damage_kind,
	HitLevel hit_level, int hit_effect_id, int hit_sound_id) {
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
	this->counterhit_type = counterhit_type;
	this->hit_status = hit_status;
	this->custom_hit_status = custom_hit_status;
	this->counterhit_status = counterhit_status;
	this->custom_counterhit_status = custom_counterhit_status;
	this->down_face_down = knockdown_face_down;
	this->continue_launch = continue_launch;
	this->juggle_start = juggle_start;
	this->juggle_increase = juggle_increase;
	this->juggle_max = juggle_max;
	this->hit_height = hit_height;
	this->damage = damage;
	this->chip_damage = chip_damage;
	this->damage_scale = damage_scale;
	this->meter_gain = meter_gain;
	this->hitlag = hitlag;
	this->blocklag = blocklag;
	this->hitstun = hitstun;
	this->blockstun = blockstun;
	this->disable_hitstun_parry = disable_hitstun_parry;
	this->pushback_ground_hit = pushback_ground_hit * 10.0f;
	this->pushback_ground_block = pushback_ground_block * 10.0f;
	this->pushback_air_x = pushback_air_x * 10.0f;
	this->pushback_air_y = pushback_air_y * 10.0f;
	this->pushback_frames = pushback_frames;
	this->launch_init_y = launch_init_y;
	this->launch_gravity_y = launch_gravity_y;
	this->launch_max_fall_speed = launch_max_fall_speed;
	this->launch_speed_x = launch_speed_x;
	this->launch_target_pos = launch_target_pos;
	this->has_launch_target_pos = has_launch_target_pos;
	this->damage_kind = damage_kind;
	this->hit_level = hit_level;
	this->hit_effect_id = hit_effect_id;
	this->hit_sound_id = hit_sound_id;
	this->active = true;
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