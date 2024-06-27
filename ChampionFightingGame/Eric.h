#pragma once
#include "Fighter.h"
#include "EricConstants.h"

const int ERIC_PROJECTILE_ID_FIREBALL = 0;

class Eric : public Fighter {
public:
	Eric();
	Eric(Player* player);
	void load_chara_status_scripts() override;
	void load_chara_effects() override;
	void load_move_scripts() override;
	void load_move_list() override;
	void load_cpu_move_info() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_status_attack() override;
	bool chara_status_attack_air() override;

	//Statuses

	void eric_status_special_fireball_start();
	void eric_enter_status_special_fireball_start();
	void eric_exit_status_special_fireball_start();

	void eric_status_special_fireball_punch();
	void eric_enter_status_special_fireball_punch();
	void eric_exit_status_special_fireball_punch();

	void eric_status_special_fireball_kick();
	void eric_enter_status_special_fireball_kick();
	void eric_exit_status_special_fireball_kick();

	void eric_status_special_slide();
	void eric_enter_status_special_slide();
	void eric_exit_status_special_slide();

	void eric_status_special_slide_followup();
	void eric_enter_status_special_slide_followup();
	void eric_exit_status_special_slide_followup();

	void eric_status_special_slide_enhanced_start();
	void eric_enter_status_special_slide_enhanced_start();
	void eric_exit_status_special_slide_enhanced_start();

	void eric_status_special_slide_enhanced();
	void eric_enter_status_special_slide_enhanced();
	void eric_exit_status_special_slide_enhanced();

	void eric_status_special_uppercut_start();
	void eric_enter_status_special_uppercut_start();
	void eric_exit_status_special_uppercut_start();

	void eric_status_special_uppercut();
	void eric_enter_status_special_uppercut();
	void eric_exit_status_special_uppercut();

	void eric_status_special_uppercut_fall();
	void eric_enter_status_special_uppercut_fall();
	void eric_exit_status_special_uppercut_fall();

	void eric_status_special_uppercut_land();
	void eric_enter_status_special_uppercut_land();
	void eric_exit_status_special_uppercut_land();

	void eric_status_special_install();
	void eric_enter_status_special_install();
	void eric_exit_status_special_install();

	void eric_status_champion_super_start();
	void eric_enter_status_champion_super_start();
	void eric_exit_status_champion_super_start();

	void eric_status_champion_super();
	void eric_enter_status_champion_super();
	void eric_exit_status_champion_super();
};