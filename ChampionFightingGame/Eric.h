#pragma once
#include "Eric.fwd.h"
#include "PlayerInfo.fwd.h"
#include "PlayerInfo.h"
#include "utils.h"

class Eric : public PlayerInfo {
public:
	int eric_int[CHARA_ERIC_INT_MAX];
	float eric_float[CHARA_ERIC_FLOAT_MAX];
	bool eric_flag[CHARA_ERIC_FLAG_MAX];

	void loadEricStatusFunctions();

	Eric();
	Eric(PlayerInfo* player_info, int id);
};