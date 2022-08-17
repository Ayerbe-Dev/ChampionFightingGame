#pragma once
#include "Fighter.h"
#include "RowanConstants.h"

class Rowan : public Fighter {
public:
	//Setup

	virtual void chara_id();
	Rowan();
	Rowan(int id, Player* player);
	void loadRoyStatusFunctions();
	void load_chara_sounds() override;
	void load_chara_effects() override;
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;

	//Statuses

	void rowan_status_special_fireball_start();
	void rowan_enter_status_special_fireball_start();
	void rowan_exit_status_special_fireball_start();
	void rowan_status_special_fireball_punch();
	void rowan_enter_status_special_fireball_punch();
	void rowan_exit_status_special_fireball_punch();
	void rowan_status_special_fireball_kick();
	void rowan_enter_status_special_fireball_kick();
	void rowan_exit_status_special_fireball_kick();
	void rowan_status_special_slide();
	void rowan_enter_status_special_slide();
	void rowan_exit_status_special_slide();
	void rowan_status_special_slide_followup();
	void rowan_enter_status_special_slide_followup();
	void rowan_exit_status_special_slide_followup();
	void rowan_status_special_uppercut_start();
	void rowan_enter_status_special_uppercut_start();
	void rowan_exit_status_special_uppercut_start();
	void rowan_status_special_uppercut();
	void rowan_enter_status_special_uppercut();
	void rowan_exit_status_special_uppercut();
	void rowan_status_special_uppercut_fall();
	void rowan_enter_status_special_uppercut_fall();
	void rowan_exit_status_special_uppercut_fall();
};