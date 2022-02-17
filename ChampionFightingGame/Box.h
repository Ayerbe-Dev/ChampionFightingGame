#pragma once
#include "utils.h"
#include <SDL.h>
#include "Box.fwd.h"
#include "BattleObject.fwd.h"
#include "GameRect.h"

class Hitbox {
public:
	BattleObject* object;
	int id;
	int multihit;
	GameRect rect;
	int hitbox_kind;
	int situation_hit;
	int attack_level;
	int clank_kind;
	float damage;
	float chip_damage;
	float counterhit_damage_mul;
	int scale;
	float meter_gain_on_hit;
	float meter_gain_on_counterhit;
	float meter_gain_on_block;
	int hitlag;
	int hitstun;
	int blocklag;
	int blockstun;
	int attack_height;
	bool unblockable;
	int juggle_set;
	int max_juggle;
	int hit_status;
	int counterhit_status;
	int counterhit_type;
	float launch_init_y;
	float launch_gravity_y;
	float launch_max_fall_speed;
	float launch_speed_x;
	float hit_pushback;
	float block_pushback;
	vec2 init_anchor;
	vec2 init_offset;
	bool use_player_pos;
	bool trade;
	bool continue_launch;
	bool can_chip_ko;

	Hitbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, vec2 anchor,
		vec2 offset, int hitbox_kind, float  meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit,
		int hitlag, int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback,
		int clank_kind, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
		float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool use_player_pos = true);

	void activate(BattleObject* object, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, vec2 anchor,
		vec2 offset, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
		int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status,
		int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x,
		bool trade, bool continue_launch, bool can_chip_ko);
	
	void update_pos(BattleObject* object, bool add_window_width = true);
	void clear();
};

class Grabbox {
public:
	BattleObject* object;
	int id;
	GameRect rect;
	int grabbox_kind;
	int situation_hit;
	u32 attacker_status_if_hit;
	u32 defender_status_if_hit;
	vec2 init_anchor;
	vec2 init_offset;
	bool use_player_pos;

	Grabbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, vec2 anchor, vec2 offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
		u32 defender_status_if_hit, bool use_player_pos = true);

	void update_pos(BattleObject* fighter);
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
	vec2 init_anchor;
	vec2 init_offset;

	Hurtbox();
	void init(BattleObject* object);
	void activate(BattleObject* object, int id, vec2 anchor, vec2 offset, int hurtbox_kind, bool is_armor, int intangible_kind);
	
	void update_pos(BattleObject *object);
	void clear();
};