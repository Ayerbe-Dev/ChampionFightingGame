#pragma once
#include "Fighter.h"
#include "DanteConstants.h"

class Dante : public Fighter {
public:
	virtual void chara_id();
	Dante();
	Dante(int id, Player* player);
	void loadDanteStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};