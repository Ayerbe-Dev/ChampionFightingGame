#pragma once
#include "ObjectInstance.h"
#include "utils.h"
#include "FighterInstanceAccessor.fwd.h"
#include "FighterInstanceAccessor.h"
#include "ParamTable.h"

class ProjectileInstance: public ObjectInstance {
public:

	int projectile_kind;
	int owner_id;
	FighterInstanceAccessor* fighter_instance_accessor;

	int projectile_int[PROJECTILE_INT_MAX];
	float projectile_float[PROJECTILE_FLOAT_MAX];
	bool projectile_flag[PROJECTILE_FLAG_MAX];

	void (ProjectileInstance::* pStatus[PROJECTILE_STATUS_MAX])();
	void (ProjectileInstance::* pEnter_status[PROJECTILE_STATUS_MAX])();
	void (ProjectileInstance::* pExit_status[PROJECTILE_STATUS_MAX])();

	void superInit(SDL_Renderer* renderer);
	void load_anim_list(SDL_Renderer* renderer);
	void load_params();
	void change_anim(string animation_name, int max_ticks = 1, int entry_frame = 0);
	void startAnimation(Animation* animation);
	void loadStatusFunctions();

	bool canStep();
	void stepAnimation();
	bool change_status(u32 new_status_kind, bool call_end_status = true, bool require_different_status = true);
	void playoutStatus();

	void new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
		float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
		int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, bool success_hit, int juggle_set, int max_juggle, int hit_status, 
		int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, bool continue_launch);

	virtual void tickOnceProjectileUnique() {};

	virtual void status_default();
	virtual void enter_status_default();
	virtual void exit_status_default();
	virtual void status_move();
	virtual void enter_status_move();
	virtual void exit_status_move();
	virtual void status_hit();
	virtual void enter_status_hit();
	virtual void exit_status_hit();

	virtual void status_roy_fireball_hover() {};
	virtual void enter_status_roy_fireball_hover() {};
	virtual void exit_status_roy_fireball_hover() {};
	virtual void status_roy_fireball_punched() {};
	virtual void enter_status_roy_fireball_punched() {};
	virtual void exit_status_roy_fireball_punched() {};
	virtual void status_roy_fireball_kicked() {};
	virtual void enter_status_roy_fireball_kicked() {};
	virtual void exit_status_roy_fireball_kicked() {};
	virtual void status_roy_fireball_fall() {};
	virtual void enter_status_roy_fireball_fall() {};
	virtual void exit_status_roy_fireball_fall() {};
	virtual void status_roy_fireball_ground() {};
	virtual void enter_status_roy_fireball_ground() {};
	virtual void exit_status_roy_fireball_ground() {};

	virtual void projectile_template_status_template() {};
	virtual void projectile_template_enter_status_template() {};
	virtual void projectile_template_exit_status_template() {};
};