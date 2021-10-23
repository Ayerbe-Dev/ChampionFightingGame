#pragma once
#include "ObjectInstance.h"
#include "utils.h"
#include "FighterInstanceAccessor.fwd.h"
#include "FighterInstanceAccessor.h"
#include "ParamTable.h"

class ProjectileInstance: public ObjectInstance {
public:
	int projectile_kind;
	FighterInstanceAccessor* fighter_instance_accessor;

	Param param_table[PARAM_TABLE_LENGTH];
	int projectile_int[PROJECTILE_INT_MAX];
	float projectile_float[PROJECTILE_FLOAT_MAX];
	bool projectile_flag[PROJECTILE_FLAG_MAX];

	void (ProjectileInstance::* pStatus[PROJECTILE_STATUS_MAX])();
	void (ProjectileInstance::* pEnter_status[PROJECTILE_STATUS_MAX])();
	void (ProjectileInstance::* pExit_status[PROJECTILE_STATUS_MAX])();

	void superInit(SDL_Renderer* renderer);
	void load_anim_list(SDL_Renderer* renderer);
	void load_params();
	void loadStatusFunctions();
	void change_anim(string animation_name, int max_ticks = 1, int entry_frame = 0);
	void startAnimation(Animation* animation);

	virtual void status_default();
	virtual void enter_status_default();
	virtual void exit_status_default();
};