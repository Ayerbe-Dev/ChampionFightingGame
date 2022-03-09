#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "utils.h"

class Priest : public Fighter {
public:
	virtual void chara_id();
	Priest();
	Priest(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadPriestStatusFunctions();
	void load_move_scripts() override;

	void (Priest::* priest_status[CHARA_PRIEST_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Priest::* priest_enter_status[CHARA_PRIEST_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Priest::* priest_exit_status[CHARA_PRIEST_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int priest_int[CHARA_PRIEST_INT_MAX];
	float priest_float[CHARA_PRIEST_FLOAT_MAX];
	bool priest_flag[CHARA_PRIEST_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};