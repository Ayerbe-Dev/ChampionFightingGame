#pragma once
#include "Fighter.fwd.h"
#include "Fighter.h"
#include "BattleObject.fwd.h"
#include "BattleObject.h"
#include "Angelica.fwd.h"
#include "utils.h"

class Angelica : public Fighter {
public:
	virtual void chara_id();
	Angelica();
	Angelica(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor);
	void loadAngelicaStatusFunctions();
	void loadCharaMoveScripts() override;

	void (Angelica::* angelica_status[CHARA_ANGELICA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Angelica::* angelica_enter_status[CHARA_ANGELICA_STATUS_MAX - FIGHTER_STATUS_MAX])();
	void (Angelica::* angelica_exit_status[CHARA_ANGELICA_STATUS_MAX - FIGHTER_STATUS_MAX])();

	int angelica_int[CHARA_ANGELICA_INT_MAX];
	float angelica_float[CHARA_ANGELICA_FLOAT_MAX];
	bool angelica_flag[CHARA_ANGELICA_FLAG_MAX];

	void chara_main() override;
	void chara_status() override;
	void chara_enter_status() override;
	void chara_exit_status() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;
	bool specific_status_attack() override;
};