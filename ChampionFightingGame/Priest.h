#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "PriestConstants.h"

class Priest : public Fighter {
public:
	virtual void chara_id();
	Priest();
	Priest(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadPriestStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};