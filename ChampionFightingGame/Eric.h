#pragma once
#include "Fighter.h"
#include "EricConstants.h"

class Eric : public Fighter {
public:
	//Setup

	virtual void chara_id();
	Eric();
	Eric(int id, PlayerInfo* player_info);
	void loadEricStatusFunctions();
	void load_move_scripts() override;
	
	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;
	
	//Statuses
	void enter_status_dash() override;
	void exit_status_dash() override;
	void enter_status_dashb() override;
	void exit_status_dashb() override;

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