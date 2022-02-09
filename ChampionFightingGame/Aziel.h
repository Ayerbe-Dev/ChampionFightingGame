#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Aziel.fwd.h"
#include "utils.h"

class Aziel : public Fighter {
public:
	virtual void chara_id();
	Aziel();
	Aziel(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadAzielStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Aziel::* aziel_status[CHARA_AZIEL_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Aziel::* aziel_enter_status[CHARA_AZIEL_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Aziel::* aziel_exit_status[CHARA_AZIEL_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int aziel_int[CHARA_AZIEL_INT_MAX];
	float aziel_float[CHARA_AZIEL_FLOAT_MAX];
	bool aziel_flag[CHARA_AZIEL_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};