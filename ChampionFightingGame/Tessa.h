#pragma once
#include "Fighter.h"
#include "TessaConstants.h"

class Tessa : public Fighter {
public:
	virtual void chara_id();
	Tessa();
	Tessa(int id, Player* player);
	void loadTessaStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};