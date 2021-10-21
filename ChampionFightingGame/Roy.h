#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
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
	virtual void chara_id();

	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];

	void loadRoyACMD();
	void loadRoyStatusFunctions();
	void set_current_move_script(string anim_name);
	void script(string name, function<void()> move_script);

	RoyScript roy_scripts[256];

	Roy();
	Roy(SDL_Renderer *renderer, int id);

	void status_parry() override;

	void roy_status_fireball_start() override;
	void roy_enter_status_fireball_start() override;
	void roy_exit_status_fireball_start() override;
};