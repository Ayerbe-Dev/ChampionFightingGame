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
	int damage;
	int hitlag;
	int hitstun;
	int blocklag;
	int blockstun;
	int attack_height;
	int meter_gain;
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
	GameCoordinate init_anchor;
	GameCoordinate init_offset;
	bool use_player_pos;

	Hitbox();
	Hitbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hitbox_kind, int situation_hit, int attack_level,
		int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable,
		bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, f32 launch_init_y, f32 launch_gravity_y,
		f32 launch_max_fall_speed, bool use_player_pos = true);
	
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