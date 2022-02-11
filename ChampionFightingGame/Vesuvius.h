#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Vesuvius.fwd.h"
#include "utils.h"

class Vesuvius : public Fighter {
public:
	virtual void chara_id();
	Vesuvius();
	Vesuvius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadVesuviusStatusFunctions();
	void load_move_scripts() override;

	void (Vesuvius::* vesuvius_status[CHARA_VESUVIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Vesuvius::* vesuvius_enter_status[CHARA_VESUVIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Vesuvius::* vesuvius_exit_status[CHARA_VESUVIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int vesuvius_int[CHARA_VESUVIUS_INT_MAX];
	float vesuvius_float[CHARA_VESUVIUS_FLOAT_MAX];
	bool vesuvius_flag[CHARA_VESUVIUS_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};