#pragma once
#include "utils.h"
#include <SDL.h>
#include "Box.fwd.h"
#include "ObjectInstance.fwd.h"
#include "GameCoordinate.h"

class Hitbox {
public:
	ObjectInstance* object_instance;
	int id;
	int multihit;
	SDL_Rect rect;
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
	bool success_hit;
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
	GameCoordinate init_anchor;
	GameCoordinate init_offset;
	bool use_player_pos;

	Hitbox();
	Hitbox(ObjectInstance* object_instance, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, 
		GameCoordinate offset, int hitbox_kind, float  meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, 
		int hitlag, int hitstun, int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback,
		int clank_kind, bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y,
		float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool use_player_pos = true);
	Hitbox(ObjectInstance* object_instance, int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor,
		GameCoordinate offset, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
		int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, bool success_hit, int juggle_set, int max_juggle, int hit_status,
		int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x);
	
	void update_connect(int multihit_index);
	void update_pos(ObjectInstance* object_instance, bool add_window_width = true);
	void clear();
};

class Grabbox {
public:
	ObjectInstance* object_instance;
	int id;
	SDL_Rect rect;
	int grabbox_kind;
	int situation_hit;
	u32 attacker_status_if_hit;
	u32 defender_status_if_hit;
	GameCoordinate init_anchor;
	GameCoordinate init_offset;
	bool use_player_pos;

	Grabbox();
	Grabbox(ObjectInstance* object_instance, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
		u32 defender_status_if_hit, bool use_player_pos = true);

	void update_pos(ObjectInstance* fighter_instance);
	void clear();
};

class Hurtbox {
public:
	ObjectInstance* object_instance;
	int id;
	SDL_Rect rect;
	int hurtbox_kind;
	bool is_armor;
	int intangible_kind;
	GameCoordinate init_anchor;
	GameCoordinate init_offset;

	Hurtbox();
	Hurtbox(ObjectInstance* object_instance, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_armor, int intangible_kind);
	
	void update_pos(ObjectInstance *object_instance);
	void clear();
};