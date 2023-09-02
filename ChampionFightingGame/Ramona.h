#pragma once
#include "Fighter.h"
#include "RamonaConstants.h"

class Ramona : public Fighter {
public:
	Ramona();
	Ramona(Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_air_status_act() override;
	bool chara_status_attack() override;
};