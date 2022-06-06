#pragma once
#include "Fighter.h"
#include "JuliusConstants.h"

class Julius : public Fighter {
public:
	virtual void chara_id();
	Julius();
	Julius(int id, Player* player);
	void loadJuliusStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};