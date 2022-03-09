#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "utils.h"

class Bruno : public Fighter {
public:
	virtual void chara_id();
	Bruno();
	Bruno(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadBrunoStatusFunctions();
	void load_move_scripts() override;

	void (Bruno::* bruno_status[CHARA_BRUNO_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Bruno::* bruno_enter_status[CHARA_BRUNO_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Bruno::* bruno_exit_status[CHARA_BRUNO_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int bruno_int[CHARA_BRUNO_INT_MAX];
	float bruno_float[CHARA_BRUNO_FLOAT_MAX];
	bool bruno_flag[CHARA_BRUNO_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};