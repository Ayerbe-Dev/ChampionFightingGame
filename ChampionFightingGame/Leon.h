#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Leon.fwd.h"
#include "utils.h"

class Leon : public Fighter {
public:
	virtual void chara_id();
	Leon();
	Leon(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadLeonStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Leon::* leon_status[CHARA_LEON_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Leon::* leon_enter_status[CHARA_LEON_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Leon::* leon_exit_status[CHARA_LEON_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int leon_int[CHARA_LEON_INT_MAX];
	float leon_float[CHARA_LEON_FLOAT_MAX];
	bool leon_flag[CHARA_LEON_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};