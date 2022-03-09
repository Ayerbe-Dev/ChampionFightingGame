#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "utils.h"

class Tessa : public Fighter {
public:
	virtual void chara_id();
	Tessa();
	Tessa(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadTessaStatusFunctions();
	void load_move_scripts() override;

	void (Tessa::* tessa_status[CHARA_TESSA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Tessa::* tessa_enter_status[CHARA_TESSA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Tessa::* tessa_exit_status[CHARA_TESSA_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int tessa_int[CHARA_TESSA_INT_MAX];
	float tessa_float[CHARA_TESSA_FLOAT_MAX];
	bool tessa_flag[CHARA_TESSA_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};