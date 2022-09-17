#pragma once
#include "Fighter.h"
#include "DanteConstants.h"

class Dante : public Fighter {
public:
	Dante();
	Dante(int id, Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};