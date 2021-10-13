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

	Hitbox();
	Hitbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hitbox_kind, int situation_hit, int attack_level,
		int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable,
		bool success_hit, int juggle_set, int max_juggle, bool use_player_pos = true);
	
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

	Grabbox();
	Grabbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
		u32 defender_status_if_hit, bool use_player_pos = true);

	void clear();
};

class Hurtbox {
public:
	PlayerInfo* player_info;
	int id;
	int orig_pos_x;
	int orig_pos_y;
	int orig_pos_w;
	int orig_pos_h;
	SDL_Rect rect;
	int hurtbox_kind;
	bool is_armor;
	int intangible_kind;

	Hurtbox();
	Hurtbox(PlayerInfo* player_info, int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool is_armor, int intangible_kind);
	void update_pos(PlayerInfo *player_info);
	void clear();
};