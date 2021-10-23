#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
#include "ProjectileInstance.fwd.h"
#include "ProjectileInstance.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "Eric.fwd.h"
#include "utils.h"

class EricScript {
public:
	string name;
	function<void()> eric_script{ []() {} };
	int id{ -1 };

	EricScript();
	EricScript(string name, function<void()> move_script, int id);
};

class Eric : public FighterInstance {
public:
	//Setup

	virtual void chara_id();
	Eric();
	Eric(SDL_Renderer* renderer, int id);
	void load_eric_params();
	void set_current_move_script(string anim_name);
	void script(string name, function<void()> move_script);
	EricScript eric_scripts[256];
	void loadEricStatusFunctions();
	void loadEricACMD();

	//Stats

	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];
	Param eric_table[PARAM_TABLE_LENGTH];
	
	bool specific_ground_status_act() override;
	
	//Statuses
};

class EricFireball : public ProjectileInstance {
public:
	EricFireball();
	EricFireball(SDL_Renderer* renderer, int id);
};