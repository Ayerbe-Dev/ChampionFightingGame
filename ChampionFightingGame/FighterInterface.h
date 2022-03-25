#pragma once
#include "Fighter.h"

Fighter* create_fighter(int chara_kind, int id, PlayerInfo* player_info);

class FighterInterface {
public:
	FighterInterface();
	FighterInterface(int chara_kind, int id, PlayerInfo *player_info);
	~FighterInterface();
	Fighter* get_fighter();
private:
	Fighter* fighter = nullptr;
};