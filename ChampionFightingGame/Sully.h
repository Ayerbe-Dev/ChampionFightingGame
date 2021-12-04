#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Sully.fwd.h"
#include "utils.h"

class Sully : public Fighter {
public:
	virtual void chara_id();
	Sully();
	Sully(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadSullyStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Sully::* sully_status[CHARA_SULLY_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Sully::* sully_enter_status[CHARA_SULLY_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Sully::* sully_exit_status[CHARA_SULLY_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int sully_int[CHARA_SULLY_INT_MAX];
	float sully_float[CHARA_SULLY_FLOAT_MAX];
	bool sully_flag[CHARA_SULLY_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};