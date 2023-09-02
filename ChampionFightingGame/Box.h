#pragma once
#include "BoxConstants.h"
#include "GameRect.h"

class BattleObject;

class Blockbox {
public:
	Blockbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, glm::vec2 anchor, glm::vec2 offset);
	void update_pos();
	void clear();

	BattleObject* object;
	GameRect rect;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};

class Hitbox {
public:
	Hitbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, HitKind hit_kind,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, ClankKind clank_kind, DamageKind ko_kind, bool continue_launch,
		bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
		float launch_max_fall_speed, float launch_speed_x);

	void update_pos();
	void clear();

	BattleObject* object;

	int id;
	int multihit;
	GameRect rect;

	float damage;
	float chip_damage;
	int damage_scale;
	float meter_gain;

	HitKind hit_kind;
	AttackLevel attack_level;
	AttackHeight attack_height;

	int hitlag;
	int blocklag;
	int hitstun;
	int blockstun;
	float hit_pushback;
	float block_pushback;

	HitStatus hit_status;
	HitStatus counterhit_status;
	CounterhitType counterhit_type;

	int juggle_start;
	int juggle_increase;
	int juggle_max;

	ClankKind clank_kind;
	DamageKind damage_kind;

	bool continue_launch;
	bool disable_hitstun_parry;

	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;

	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};

class Hurtbox {
public:
	Hurtbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, 
		HurtboxKind hurtbox_kind, bool is_armor, IntangibleKind intangible_kind);

	void update_pos();
	void clear();

	BattleObject* object;
	int id;
	GameRect rect;
	HurtboxKind hurtbox_kind;
	bool is_armor;
	IntangibleKind intangible_kind;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};

class Grabbox {
public:
	Grabbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, 
		GrabboxKind grabbox_kind, HitKind hit_kind, unsigned int attacker_status_if_hit,
		unsigned int defender_status_if_hit);

	void update_pos();
	void clear();

	BattleObject* object;
	int id;
	GameRect rect;
	GrabboxKind grabbox_kind;
	HitKind hit_kind;
	unsigned int attacker_status_if_hit;
	unsigned int defender_status_if_hit;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};

class Pushbox {
public:
	Pushbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset);
	void update_pos();
	void clear();

	BattleObject* object;
	int id;
	GameRect rect;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;
};