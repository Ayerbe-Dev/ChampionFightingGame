#pragma once
#include "CollisionBoxFields.h"
#include "WorldTexture.h"

class BattleObject;
class Fighter;

class CollisionBox {
public:
	CollisionBox();

	void update_pos();
	void clear();

	BattleObject* object;
	WorldTexture world_tex;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	glm::vec2 anchor;
	glm::vec2 offset;
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
		CollisionKind collision_kind, HitResult hit_result, HitStatus hit_status, 
		unsigned int custom_hit_status, HitMove hit_move, HitFlag hit_flags, 
		CriticalCondition critical_condition, HitStatus critical_status, 
		unsigned int custom_critical_status, HitMove critical_hit_move, 
		HitFlag critical_hit_flags, HitHeight hit_height, DamageKind damage_kind, 
		std::string hit_effect, std::string hit_sound
	);

	int id;
	int multihit;
	CollisionKind collision_kind;
	HitResult hit_result;
	HitStatus hit_status;
	unsigned int custom_hit_status;
	HitMove hit_move;
	HitFlag hit_flags;
	CriticalCondition critical_condition;
	HitStatus critical_status;
	unsigned int custom_critical_status;
	HitMove critical_hit_move;
	HitFlag critical_hit_flags;
	HitHeight hit_height;
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
		int hitlag, int hitstun, std::string hit_anim, HitMove hit_move, DamageKind damage_kind, 
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
	std::string hit_anim;
	HitMove hit_move;
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

float get_rect_intersection(CollisionBox& RectA, CollisionBox& RectB);
float get_rect_intersection(glm::vec2 c1a, glm::vec2 c2a, glm::vec2 c1b, glm::vec2 c2b);
bool is_rect_collide(CollisionBox& RectA, CollisionBox& RectB);
bool is_rect_collide(glm::vec2 c1a, glm::vec2 c2a, glm::vec2 c1b, glm::vec2 c2b);