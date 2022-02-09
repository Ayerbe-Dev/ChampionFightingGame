#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Julius.fwd.h"
#include "utils.h"

class Julius : public Fighter {
public:
	virtual void chara_id();
	Julius();
	Julius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadJuliusStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Julius::* julius_status[CHARA_JULIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Julius::* julius_enter_status[CHARA_JULIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Julius::* julius_exit_status[CHARA_JULIUS_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int julius_int[CHARA_JULIUS_INT_MAX];
	float julius_float[CHARA_JULIUS_FLOAT_MAX];
	bool julius_flag[CHARA_JULIUS_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};