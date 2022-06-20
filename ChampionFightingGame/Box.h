#pragma once
#include "BoxConstants.h"
#include "GameRect.h"

class BattleObject;

class Blockbox {
public:
	BattleObject* object;
	GameRect rect;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;

	Blockbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, glm::vec2 anchor, glm::vec2 offset);
	void update_pos();
	void clear();
};

class Hitbox {
public:
	BattleObject* object;

	int id;
	int multihit;
	GameRect rect;

	float damage;
	float chip_damage;
	int damage_scale;
	float meter_gain;

	SituationHit situation_hit;
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
	KoKind ko_kind;

	bool continue_launch;
	bool disable_hitstun_parry;

	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;

	bool use_player_pos;
	bool trade;

	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;

	Hitbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, SituationHit situation_hit,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, ClankKind clank_kind, KoKind ko_kind, bool continue_launch,
		bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y, 
		float launch_max_fall_speed, float launch_speed_x, bool use_player_pos
	);
	void activate(BattleObject* object, int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, SituationHit situation_hit,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, bool trade, KoKind ko_kind, bool continue_launch, bool disable_hitstun_parry,
		float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x
	);
	
	void update_pos();
	void clear();
};

class Grabbox {
public:
	BattleObject* object;
	int id;
	GameRect rect;
	int grabbox_kind;
	int situation_hit;
	unsigned int attacker_status_if_hit;
	unsigned int defender_status_if_hit;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool use_player_pos;
	bool active;

	Grabbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, int grabbox_kind, int situation_hit, unsigned int attacker_status_if_hit,
		unsigned int defender_status_if_hit, bool use_player_pos = true);

	void update_pos();
	void clear();
};

class Hurtbox {
public:
	BattleObject* object;
	int id;
	GameRect rect;
	int hurtbox_kind;
	bool is_armor;
	int intangible_kind;
	glm::vec2 init_anchor;
	glm::vec2 init_offset;
	bool active;

	Hurtbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, glm::vec2 anchor, glm::vec2 offset, int hurtbox_kind, bool is_armor, int intangible_kind);
	
	void update_pos();
	void clear();
};