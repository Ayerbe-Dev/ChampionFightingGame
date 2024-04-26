#pragma once
#include "Fighter.h"
#include "RowanConstants.h"

const int ROWAN_PROJECTILE_ID_FIREBALL = 0;

class Rowan : public Fighter {
public:
	Rowan();
	Rowan(Player* player);
	void load_chara_status_scripts() override;
	void load_chara_effects() override;
	void load_move_scripts() override;
	void load_move_list() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_status_attack() override;
	bool chara_status_attack_air() override;

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

	void rowan_status_special_slide_enhanced_start();
	void rowan_enter_status_special_slide_enhanced_start();
	void rowan_exit_status_special_slide_enhanced_start();

	void rowan_status_special_slide_enhanced();
	void rowan_enter_status_special_slide_enhanced();
	void rowan_exit_status_special_slide_enhanced();
	
	void rowan_status_special_uppercut_start();
	void rowan_enter_status_special_uppercut_start();
	void rowan_exit_status_special_uppercut_start();
	
	void rowan_status_special_uppercut();
	void rowan_enter_status_special_uppercut();
	void rowan_exit_status_special_uppercut();
	
	void rowan_status_special_uppercut_fall();
	void rowan_enter_status_special_uppercut_fall();
	void rowan_exit_status_special_uppercut_fall();
	
	void rowan_status_special_uppercut_land();
	void rowan_enter_status_special_uppercut_land();
	void rowan_exit_status_special_uppercut_land();
	
	void rowan_status_special_install();
	void rowan_enter_status_special_install();
	void rowan_exit_status_special_install();
};