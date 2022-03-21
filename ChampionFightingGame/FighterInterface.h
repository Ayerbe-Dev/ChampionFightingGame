#pragma once
#include "Fighter.h"

Fighter* create_fighter(int chara_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);

class FighterInterface {
public:
	FighterInterface();
	FighterInterface(int chara_kind, int id, PlayerInfo *player_info, FighterAccessor* fighter_accessor);
	~FighterInterface();
	Fighter* get_fighter();
private:
	Fighter* fighter = nullptr;
};