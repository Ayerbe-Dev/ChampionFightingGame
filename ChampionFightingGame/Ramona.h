#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "Object.fwd.h"
#include "Object.h"
#include "Ramona.fwd.h"
#include "utils.h"

class Ramona : public Fighter {
public:
	virtual void chara_id();
	Ramona();
	Ramona(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadRamonaStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Ramona::* ramona_status[CHARA_RAMONA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Ramona::* ramona_enter_status[CHARA_RAMONA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Ramona::* ramona_exit_status[CHARA_RAMONA_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int ramona_int[CHARA_RAMONA_INT_MAX];
	float ramona_float[CHARA_RAMONA_FLOAT_MAX];
	bool ramona_flag[CHARA_RAMONA_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};