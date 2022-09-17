#pragma once
#include "Fighter.h"
#include "AngelicaConstants.h"

class Angelica : public Fighter {
public:
	Angelica();
	Angelica(int id, Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};