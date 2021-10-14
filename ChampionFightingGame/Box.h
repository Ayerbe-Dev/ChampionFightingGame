#pragma once
#include "utils.h"
#include <SDL.h>
#include "Box.fwd.h"
#include "PlayerInfo.fwd.h"
#include "GameCoordinate.h"

class Hitbox {
public:
	PlayerInfo* player_info;
	int id;
	SDL_Rect rect;
	int hitbox_kind;
	int situation_hit;
	int attack_level;
	int clank_kind;
	f32 damage;
	f32 chip_damage;
	f32 counterhit_damage_mul;
	f32 meter_gain_on_hit;
	f32 meter_gain_on_counterhit;
	f32 meter_gain_on_block;
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
	f32 launch_init_y;
	f32 launch_gravity_y;
	f32 launch_max_fall_speed;
	f32 launch_speed_x;
	GameCoordinate init_anchor;
	GameCoordinate init_offset;
	bool use_player_pos;

	Hitbox();
	Hitbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hitbox_kind, int situation_hit, int attack_level,
		int clank_kind, f32 damage, f32 chip_damage, f32 counterhit_damage_mul, f32 meter_gain_on_hit, f32 meter_gain_on_counterhit, 
		f32 meter_gain_on_block, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, bool unblockable, bool success_hit,
		int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, f32 launch_init_y, 
		f32 launch_gravity_y, f32 launch_max_fall_speed, f32 launch_speed_x, bool use_player_pos = true);
	
	void update_connect();
	void update_pos(PlayerInfo* player_info);
	void clear();
};

class Grabbox {
public:
	PlayerInfo* player_info;
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
	Grabbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
		u32 defender_status_if_hit, bool use_player_pos = true);

	void update_pos(PlayerInfo* player_info);
	void clear();
};

class Hurtbox {
public:
	PlayerInfo* player_info;
	int id;
	SDL_Rect rect;
	int hurtbox_kind;
	bool is_armor;
	int intangible_kind;
	GameCoordinate init_anchor;
	GameCoordinate init_offset;

	Hurtbox();
	Hurtbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_armor, int intangible_kind);
	
	void update_pos(PlayerInfo *player_info);
	void clear();
};