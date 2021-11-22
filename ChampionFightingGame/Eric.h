#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Projectile.fwd.h"
#include "Projectile.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Eric.fwd.h"
#include "utils.h"

class Eric : public Fighter {
public:
	//Setup

	virtual void chara_id();
	Eric();
	Eric(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadEricStatusFunctions();
	void loadCharaMoveScripts() override;

	//Stats

	void (Eric::* eric_status[CHARA_ERIC_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Eric::* eric_enter_status[CHARA_ERIC_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Eric::* eric_exit_status[CHARA_ERIC_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];
	
	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

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