#pragma once
#include "Eric.fwd.h"
#include "PlayerInfo.fwd.h"
#include "PlayerInfo.h"
#include "utils.h"

class EricScript {
public:
	string name;
	void (EricScript::* move_script)();

	EricScript();
	EricScript(string name, function<void()> move_script);
};

class Eric : public PlayerInfo {
public:
	void chara_id();

	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];

	void loadEricStatusFunctions();
//	void set_current_move_script(string anim_name);

	EricScript eric_scripts[256];

	Eric();
	Eric(PlayerInfo* player_info, int id);
};