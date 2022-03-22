#pragma once
#include "Fighter.h"

Fighter* create_fighter(int chara_kind, int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager);

class FighterInterface {
public:
	FighterInterface();
	FighterInterface(int chara_kind, int id, PlayerInfo *player_info, BattleObjectManager* battle_object_manager);
	~FighterInterface();
	Fighter* get_fighter();
private:
	Fighter* fighter = nullptr;
};