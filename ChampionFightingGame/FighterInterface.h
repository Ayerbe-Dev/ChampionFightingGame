#pragma once
#include "Fighter.h"

Fighter* create_fighter(int chara_kind, int id, Player* player);

class FighterInterface {
public:
	FighterInterface();
	FighterInterface(int chara_kind, int id, Player *player);
	~FighterInterface();
	Fighter* get_fighter();
private:
	Fighter* fighter = nullptr;
};