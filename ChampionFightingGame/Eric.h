#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
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
	void chara_id();

	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];

	void loadEricACMD();
	void loadEricStatusFunctions();
	void set_current_move_script(string anim_name);
	void script(string name, function<void()> move_script);

	EricScript eric_scripts[256];

	Eric();
	Eric(SDL_Renderer *renderer, int id);
};