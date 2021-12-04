#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Zyair.fwd.h"
#include "utils.h"

class Zyair : public Fighter {
public:
	virtual void chara_id();
	Zyair();
	Zyair(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadZyairStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Zyair::* zyair_status[CHARA_ZYAIR_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Zyair::* zyair_enter_status[CHARA_ZYAIR_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Zyair::* zyair_exit_status[CHARA_ZYAIR_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int zyair_int[CHARA_ZYAIR_INT_MAX];
	float zyair_float[CHARA_ZYAIR_FLOAT_MAX];
	bool zyair_flag[CHARA_ZYAIR_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};