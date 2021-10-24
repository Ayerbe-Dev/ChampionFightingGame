#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
#include "ProjectileInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "Roy.fwd.h"
#include "utils.h"

class RoyScript {
public:
	string name;
	function<void()> roy_script{ [](){} };
	int id{ -1 };

	RoyScript();
	RoyScript(string name, function<void()> move_script, int id);
};

class Roy : public FighterInstance {
public:
	//Setup

	virtual void chara_id();
	Roy();
	Roy(SDL_Renderer* renderer, int id);
	void load_roy_params();
	void set_current_move_script(string anim_name);
	void script(string name, function<void()> move_script);
	RoyScript roy_scripts[256];
	void loadRoyStatusFunctions();
	void loadRoyACMD();

	//Stats

	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];
	Param roy_table[PARAM_TABLE_LENGTH];

	bool specific_ground_status_act() override;

	bool specific_status_attack() override;

	//Statuses

	void roy_status_fireball_start() override;
	void roy_enter_status_fireball_start() override;
	void roy_exit_status_fireball_start() override;
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

class RoyFireball : public ProjectileInstance {
public:
	RoyFireball();
	RoyFireball(SDL_Renderer* renderer, int id);

	void load_roy_fireball_params();
	void set_current_move_script(string anim_name);
	void script(string name, function<void()> move_script);
	RoyScript roy_fireball_scripts[256];
	Param roy_fireball_table[PARAM_TABLE_LENGTH];
	void loadRoyFireballStatusFunctions();
	void loadRoyFireballACMD();

	virtual void status_default() override;
};