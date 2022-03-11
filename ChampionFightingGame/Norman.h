#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "NormanConstants.h"

class Norman : public Fighter {
public:
	virtual void chara_id();
	Norman();
	Norman(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadNormanStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};