#pragma once
#include "CollisionBoxConstants.h"
#include "GameRect.h"

class BattleObject;

class CollisionBox {
public:
	CollisionBox();

	void update_pos();
	void clear();

	BattleObject* object;
	GameRect rect;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};

class Blockbox : public CollisionBox {
public:
	Blockbox();
	void init(BattleObject* object);
	void activate(glm::vec2 anchor, glm::vec2 offset);
};

class Hitbox : public CollisionBox {
public:
	Hitbox();
	void init(BattleObject* object);
	void activate(int id, int multihit, glm::vec2 anchor, glm::vec2 offset, CollisionKind collision_kind,
		CounterhitType counterhit_type, HitStatus hit_status, unsigned int custom_hit_status, 
		HitStatus counterhit_status, unsigned int custom_counterhit_status, bool knockdown_face_down,
		bool continue_launch, int juggle_start, int juggle_increase, int juggle_max, HitHeight hit_height, 
		float damage, float chip_damage, int damage_scale, float meter_gain, int hitlag, int blocklag, 
		int hitstun, int blockstun, bool disable_hitstun_parry, float pushback_ground_hit, 
		float pushback_ground_block, float pushback_air_x, float pushback_air_y, int pushback_frames, 
		float launch_init_y, float launch_gravity, float launch_max_fall_speed, float launch_speed_x, 
		glm::vec3 launch_target_pos, bool has_launch_target_pos, DamageKind damage_kind, 
		HitLevel hit_level, int hit_effect_id, int hit_sound_id);

	int id;
	int multihit;
	CollisionKind collision_kind;
	CounterhitType counterhit_type;
	HitStatus hit_status;
	unsigned int custom_hit_status;
	HitStatus counterhit_status;
	unsigned int custom_counterhit_status;
	bool down_face_down;
	bool continue_launch;
	int juggle_start;
	int juggle_increase;
	int juggle_max;
	HitHeight hit_height;
	float damage;
	float chip_damage;
	int damage_scale;
	float meter_gain;
	int hitlag;
	int blocklag;
	int hitstun;
	int blockstun;
	bool disable_hitstun_parry;
	float pushback_ground_hit;
	float pushback_ground_block;
	float pushback_air_x;
	float pushback_air_y;
	int pushback_frames;
	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;
	glm::vec3 launch_target_pos;
	bool has_launch_target_pos;
	DamageKind damage_kind;
	HitLevel hit_level;
	int hit_effect_id;
	int hit_sound_id;
};

class Hurtbox : public CollisionBox {
public:
	Hurtbox();
	void init(BattleObject* object);
	void activate(int id, glm::vec2 anchor, glm::vec2 offset, 
		HurtboxKind hurtbox_kind, int armor_hits, IntangibleKind intangible_kind);

	int id;
	HurtboxKind hurtbox_kind;
	int armor_hits;
	IntangibleKind intangible_kind;
};

class Grabbox : public CollisionBox {
public:
	Grabbox();
	void init(BattleObject* object);
	void activate(int id, glm::vec2 anchor, glm::vec2 offset, 
		GrabboxKind grabbox_kind, CollisionKind hit_kind, unsigned int attacker_status_if_hit,
		unsigned int defender_status_if_hit);

	int id;
	GrabboxKind grabbox_kind;
	CollisionKind collision_kind;
	unsigned int attacker_status_if_hit;
	unsigned int defender_status_if_hit;
};

class Pushbox : public CollisionBox {
public:
	Pushbox();
	void init(BattleObject* object);
	void activate(int id, glm::vec2 anchor, glm::vec2 offset);

	int id;
};