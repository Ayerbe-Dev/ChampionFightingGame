#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "RamonaConstants.h"

class Ramona : public Fighter {
public:
	virtual void chara_id();
	Ramona();
	Ramona(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadRamonaStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};