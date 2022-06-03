#pragma once
#include "Fighter.h"
#include "ZyairConstants.h"

class Zyair : public Fighter {
public:
	virtual void chara_id();
	Zyair();
	Zyair(int id, Player* player);
	void loadZyairStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};