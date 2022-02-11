#pragma once
#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Alejandro.fwd.h"
#include "utils.h"

class Alejandro : public Fighter {
public:
	virtual void chara_id();
	Alejandro();
	Alejandro(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadAlejandroStatusFunctions();
	void load_move_scripts() override;

	void (Alejandro::* alejandro_status[CHARA_ALEJANDRO_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Alejandro::* alejandro_enter_status[CHARA_ALEJANDRO_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Alejandro::* alejandro_exit_status[CHARA_ALEJANDRO_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int alejandro_int[CHARA_ALEJANDRO_INT_MAX];
	float alejandro_float[CHARA_ALEJANDRO_FLOAT_MAX];
	bool alejandro_flag[CHARA_ALEJANDRO_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};