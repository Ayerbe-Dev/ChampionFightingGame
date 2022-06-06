#pragma once
#include "Fighter.h"
#include "RoyConstants.h"

class Roy : public Fighter {
public:
	//Setup

	virtual void chara_id();
	Roy();
	Roy(int id, Player* player);
	void loadRoyStatusFunctions();
	void load_chara_sounds() override;
	void load_chara_effects() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;

	//Statuses

	void roy_status_special_fireball_start();
	void roy_enter_status_special_fireball_start();
	void roy_exit_status_special_fireball_start();
	void roy_status_special_fireball_punch();
	void roy_enter_status_special_fireball_punch();
	void roy_exit_status_special_fireball_punch();
	void roy_status_special_fireball_kick();
	void roy_enter_status_special_fireball_kick();
	void roy_exit_status_special_fireball_kick();
	void roy_status_special_slide();
	void roy_enter_status_special_slide();
	void roy_exit_status_special_slide();
	void roy_status_special_slide_followup();
	void roy_enter_status_special_slide_followup();
	void roy_exit_status_special_slide_followup();
	void roy_status_special_uppercut_start();
	void roy_enter_status_special_uppercut_start();
	void roy_exit_status_special_uppercut_start();
	void roy_status_special_uppercut();
	void roy_enter_status_special_uppercut();
	void roy_exit_status_special_uppercut();
	void roy_status_special_uppercut_fall();
	void roy_enter_status_special_uppercut_fall();
	void roy_exit_status_special_uppercut_fall();
};