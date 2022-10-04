#pragma once
#include "Fighter.h"
#include "VesuviusConstants.h"

class Vesuvius : public Fighter {
public:
	Vesuvius();
	Vesuvius(Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};