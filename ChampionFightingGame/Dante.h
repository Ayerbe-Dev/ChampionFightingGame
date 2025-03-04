#pragma once
#include "Fighter.h"
#include "DanteConstants.h"

class Dante : public Fighter {
public:
	Dante();
	Dante(Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;
	void load_move_list() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_status_attack() override;
	bool chara_status_attack_air() override;
};