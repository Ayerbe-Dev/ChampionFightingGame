#pragma once
#include "PlayerInfo.fwd.h"
#include "PlayerInfo.h"
#include "Roy.fwd.h"
#include "utils.h"

class RoyScript {
public:
	string name;
	void (RoyScript::* move_script)();
	int id{ -1 };

	RoyScript();
	RoyScript(string name, function<void()> move_script, int id);
};

class Roy : public PlayerInfo {
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
};