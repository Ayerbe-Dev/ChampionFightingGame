#pragma once
#include "Fighter.h"
#include "EricConstants.h"

const int ERIC_PROJECTILE_ID_FIREBALL = 0;

class Eric : public Fighter {
public:
	Eric();
	Eric(Player* player);
	void load_chara_status_scripts() override;
	void load_move_scripts() override;
	void load_move_list() override;

	void chara_main() override;

	bool chara_ground_status_act() override;
	bool chara_status_attack() override;
	bool chara_status_attack_air() override;

	//Statuses
	void enter_status_dash_f() override;
	void exit_status_dash_f() override;
	void enter_status_dash_b() override;
	void exit_status_dash_b() override;

	void eric_status_special_uppercut_start();
	void eric_enter_status_special_uppercut_start();
	void eric_exit_status_special_uppercut_start();
	void eric_status_special_uppercut();
	void eric_enter_status_special_uppercut();
	void eric_exit_status_special_uppercut();
	void eric_status_special_uppercut_fall();
	void eric_enter_status_special_uppercut_fall();
	void eric_exit_status_special_uppercut_fall();
};