#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "utils.h"

class Atlas : public Fighter {
public:
	virtual void chara_id();
	Atlas();
	Atlas(int id, PlayerInfo *player_info, FighterAccessor* fighter_accessor);
	void loadAtlasStatusFunctions();
	void load_move_scripts() override;

	void (Atlas::* atlas_status[CHARA_ATLAS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Atlas::* atlas_enter_status[CHARA_ATLAS_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Atlas::* atlas_exit_status[CHARA_ATLAS_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int atlas_int[CHARA_ATLAS_INT_MAX];
	float atlas_float[CHARA_ATLAS_FLOAT_MAX];
	bool atlas_flag[CHARA_ATLAS_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;
};