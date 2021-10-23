#pragma once
#include "ObjectInstance.h"
#include "utils.h"
#include "FighterInstanceAccessor.fwd.h"
#include "FighterInstanceAccessor.h"

class ProjectileInstance: public ObjectInstance {
public:
	int projectile_kind;
	FighterInstanceAccessor* fighter_instance_accessor;

	int projectile_int[PROJECTILE_INT_MAX];
	float projectile_float[PROJECTILE_FLOAT_MAX];
	bool projectile_flag[PROJECTILE_FLAG_MAX];

	void load_anim_list(SDL_Renderer* renderer);
};