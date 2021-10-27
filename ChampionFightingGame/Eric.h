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
	Eric(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);
	void loadEricStatusFunctions();
	void loadEricACMD();

	//Stats

	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];
	
	bool specific_ground_status_act() override;
	
	//Statuses
	void enter_status_dash() override;
	void exit_status_dash() override;
	void enter_status_dashb() override;
	void exit_status_dashb() override;
};