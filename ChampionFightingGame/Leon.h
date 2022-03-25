#pragma once
#include "Fighter.h"
#include "LeonConstants.h"

class Leon : public Fighter {
public:
	virtual void chara_id();
	Leon();
	Leon(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager);
	void loadLeonStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};