#pragma once
#include "Fighter.h"
#include "RookConstants.h"

class Rook : public Fighter {
public:
	virtual void chara_id();
	Rook();
	Rook(int id, Player* player);
	void loadRookStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};