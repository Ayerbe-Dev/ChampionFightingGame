#pragma once
#include "Fighter.h"
#include "BrunoConstants.h"

class Bruno : public Fighter {
public:
	virtual void chara_id();
	Bruno();
	Bruno(int id, Player* player);
	void loadBrunoStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};