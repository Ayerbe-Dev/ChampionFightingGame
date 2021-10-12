using namespace std;
#include <iostream>
#include <sstream>
#include "PlayerInfo.h"
#include "utils.h"
#pragma once

class Hitbox {
public:
	PlayerInfo* player_info;
	int id;
	int x0;
	int x1;
	int y0;
	int y1;
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
	Hitbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, bool raw_coords, int hitbox_kind, int situation_hit, int attack_level, 
		int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable, 
		bool success_hit, int juggle_set, int max_juggle);
};

class Grabbox {
public:
	PlayerInfo* player_info;
	int id;
	int x0;
	int x1;
	int y0;
	int y1;
	int grabbox_kind;
	int situation_hit;
	u32 attacker_status_if_hit;
	u32 defender_status_if_hit;

	Grabbox();
	Grabbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, bool raw_coords, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit,
		u32 defender_status_if_hit);
};

class Hurtbox {
public:
	PlayerInfo* player_info;
	int id;
	int x0;
	int x1;
	int y0;
	int y1;
	int hurtbox_kind;
	bool is_invincible;
	bool is_armor;
	int intangible_kind;

	Hurtbox();
	Hurtbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, int hurtbox_kind, bool is_invincible, bool is_armor, int intangible_kind);
};