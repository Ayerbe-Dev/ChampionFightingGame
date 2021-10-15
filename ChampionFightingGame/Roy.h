#pragma once
#include "Roy.fwd.h"
#include "PlayerInfo.fwd.h"
#include "PlayerInfo.h"
#include "utils.h"

class Roy : public PlayerInfo {
public:
	int roy_int[CHARA_ROY_INT_MAX];
	float roy_float[CHARA_ROY_FLOAT_MAX];
	bool roy_flag[CHARA_ROY_FLAG_MAX];

	void loadRoyStatusFunctions();

	Roy();
	Roy(PlayerInfo* player_info, int id);
};