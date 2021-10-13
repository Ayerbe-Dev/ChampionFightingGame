using namespace std;
#include <iostream>
#include <sstream>
#include "PlayerInfo.h"
#include "utils.h"
#pragma once

class Hitbox {
public:
//	PlayerInfo* player_info;
	int id;
	int x0;
	int x1;
	int y0;
	int y1;
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

//	Hitbox();
//	Hitbox(PlayerInfo* player_info, int id, int x0, int x1, int y0, int y1, bool raw_coords, int hitbox_kind, int situation_hit, int attack_level, 
//		int clank_kind, int damage, int hitlag, int hitstun, int blocklag, int blockstun, int attack_height, int meter_gain, bool unblockable, 
//		bool success_hit, int juggle_set, int max_juggle);
//	void clear_hitbox();
};