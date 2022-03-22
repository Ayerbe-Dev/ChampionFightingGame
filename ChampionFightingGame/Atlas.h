#pragma once
#include "Fighter.h"
#include "BattleObject.h"
#include "AtlasConstants.h"

class Atlas : public Fighter {
public:
	virtual void chara_id();
	Atlas();
	Atlas(int id, PlayerInfo *player_info, BattleObjectManager* battle_object_manager);
	void loadAtlasStatusFunctions();
	void load_move_scripts() override;

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;
};