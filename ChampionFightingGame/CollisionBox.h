#pragma once
#include "GameRect.h"
#include "CollisionBoxFields.h"

class BattleObject;
class Fighter;

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
	void activate(int id, int multihit, glm::vec2 anchor, glm::vec2 offset, 
		CollisionKind collision_kind, float damage, float chip_damage, int damage_scale, 
		float meter_gain, int hitlag, int hitstun, int blocklag, int blockstun, 
		HitStatus hit_status, unsigned int custom_hit_status, HitResult hit_result, 
		HitFlag hit_flags, CriticalCondition critical_condition, HitStatus critical_status, 
		unsigned int custom_critical_status, HitResult critical_hit_result, 
		HitFlag critical_hit_flags, int juggle_start, int juggle_increase, int juggle_max, 
		HitHeight hit_height, DamageKind damage_kind, std::string hit_effect, 
		std::string hit_sound
	);

	int id;
	int multihit;
	CollisionKind collision_kind;
	HitStatus hit_status;
	unsigned int custom_hit_status;
	HitResult hit_result;
	HitFlag hit_flags;
	CriticalCondition critical_condition;
	HitStatus critical_status;
	unsigned int custom_critical_status;
	HitResult critical_hit_result;
	HitFlag critical_hit_flags;
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
	DamageKind damage_kind;
	std::string hit_effect;
	std::string hit_sound;
};

class DefiniteHitbox { 
public:
	DefiniteHitbox();
	void set_properties(BattleObject* object, Fighter* defender, Hitbox* hitbox);
	void set_properties(BattleObject* object, Fighter* defender, unsigned int hit_status, HitFlag hit_flags, int juggle_start,
		int juggle_increase, float damage, int damage_scale, float meter_gain,
		int hitlag, int hitstun, HitResult hit_result, DamageKind damage_kind, 
		std::string hit_effect, std::string hit_sound
	);
	void activate();
	void clear();
	
	Fighter* attacker;
	Fighter* defender;
	unsigned int hit_status;
	HitFlag hit_flags;
	int juggle_start;
	int juggle_increase;
	float damage;
	int damage_scale;
	float meter_gain;
	int hitlag;
	int hitstun;
	HitResult hit_result;
	DamageKind damage_kind;
	std::string hit_effect;
	std::string hit_sound;
	bool active;
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