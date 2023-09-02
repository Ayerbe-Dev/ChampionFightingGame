#pragma once
#include "Fighter.h"
#include "AtlasConstants.h"

class Atlas : public Fighter {
public:
	Atlas();
	Atlas(Player *player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_air_status_act() override;

	bool chara_status_attack() override;
};