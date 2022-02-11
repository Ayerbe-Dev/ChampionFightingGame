#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Nightsaber.fwd.h"
#include "utils.h"

class Nightsaber : public Fighter {
public:
	virtual void chara_id();
	Nightsaber();
	Nightsaber(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadNightsaberStatusFunctions();
	void load_move_scripts() override;

	void (Nightsaber::* nightsaber_status[CHARA_NIGHTSABER_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Nightsaber::* nightsaber_enter_status[CHARA_NIGHTSABER_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Nightsaber::* nightsaber_exit_status[CHARA_NIGHTSABER_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int nightsaber_int[CHARA_NIGHTSABER_INT_MAX];
	float nightsaber_float[CHARA_NIGHTSABER_FLOAT_MAX];
	bool nightsaber_flag[CHARA_NIGHTSABER_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};