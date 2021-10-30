#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "Roy.fwd.h"
#include "utils.h"

class Roy : public FighterInstance {
public:
	//Setup

	virtual void chara_id();
	Roy();
	Roy(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);
	void loadRoyStatusFunctions();
	void loadRoyACMD();

	//Stats

	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;

	//Statuses

	void roy_status_special_fireball_start() override;
	void roy_enter_status_special_fireball_start() override;
	void roy_exit_status_special_fireball_start() override;
	void roy_status_special_fireball_punch() override;
	void roy_enter_status_special_fireball_punch() override;
	void roy_exit_status_special_fireball_punch() override;
	void roy_status_special_fireball_kick() override;
	void roy_enter_status_special_fireball_kick() override;
	void roy_exit_status_special_fireball_kick() override;
	void roy_status_special_uppercut_start() override;
	void roy_enter_status_special_uppercut_start() override;
	void roy_exit_status_special_uppercut_start() override;
	void roy_status_special_uppercut() override;
	void roy_enter_status_special_uppercut() override;
	void roy_exit_status_special_uppercut() override;
	void roy_status_special_uppercut_fall() override;
	void roy_enter_status_special_uppercut_fall() override;
	void roy_exit_status_special_uppercut_fall() override;
};