#pragma once
#include "ObjectInstance.h"
#include "utils.h"
#include "FighterInstanceAccessor.fwd.h"
#include "FighterInstanceAccessor.h"

class ProjectileInstance: public ObjectInstance {
public:
	int projectile_kind;
	FighterInstanceAccessor* fighter_instance_accessor;

	int chara_int[CHARA_INT_MAX];
	float chara_float[CHARA_FLOAT_MAX];
	bool chara_flag[CHARA_FLAG_MAX];
};