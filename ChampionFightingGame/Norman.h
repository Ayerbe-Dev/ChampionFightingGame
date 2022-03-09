#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "utils.h"

class Norman : public Fighter {
public:
	virtual void chara_id();
	Norman();
	Norman(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadNormanStatusFunctions();
	void load_move_scripts() override;

	void (Norman::* norman_status[CHARA_NORMAN_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Norman::* norman_enter_status[CHARA_NORMAN_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Norman::* norman_exit_status[CHARA_NORMAN_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int norman_int[CHARA_NORMAN_INT_MAX];
	float norman_float[CHARA_NORMAN_FLOAT_MAX];
	bool norman_flag[CHARA_NORMAN_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};