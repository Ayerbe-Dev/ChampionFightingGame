#pragma once
#include "Roy.fwd.h"
#include "PlayerInfo.fwd.h"
#include "PlayerInfo.h"
#include "utils.h"

class RoyScript {
public:
	string name;
	void (RoyScript::* move_script)();

	RoyScript();
	RoyScript(string name, function<void()> move_script);
};

class Roy : public PlayerInfo {
public:
	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];

	void loadRoyStatusFunctions();
	void set_current_move_script(string anim_name); //Will go in child class

	RoyScript roy_scripts[256]; //Will go in child class in the future

	Roy();
	Roy(PlayerInfo* player_info, int id);
};