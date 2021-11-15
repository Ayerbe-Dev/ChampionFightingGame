#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Roy.fwd.h"
#include "utils.h"

class Roy : public Fighter {
public:
	//Setup

	virtual void chara_id();
	Roy();
	Roy(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadRoyStatusFunctions();
	void loadCharaSounds() override;
	void loadRoyACMD();

	//Stats

	void (Roy::* roy_status[CHARA_ROY_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Roy::* roy_enter_status[CHARA_ROY_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Roy::* roy_exit_status[CHARA_ROY_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

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