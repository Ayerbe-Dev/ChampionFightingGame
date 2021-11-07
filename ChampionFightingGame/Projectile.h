#pragma once
#include "Object.h"
#include "utils.h"
#include "FighterAccessor.fwd.h"
#include "FighterAccessor.h"
#include "ParamTable.h"

class Projectile: public Object {
public:

	int projectile_kind;
	int owner_id;
	FighterAccessor* fighter_accessor;

	int projectile_int[PROJECTILE_INT_MAX];
	float projectile_float[PROJECTILE_FLOAT_MAX];
	bool projectile_flag[PROJECTILE_FLAG_MAX];

	void (Projectile::* status_script[PROJECTILE_STATUS_MAX])();
	void (Projectile::* enter_status_script[PROJECTILE_STATUS_MAX])();
	void (Projectile::* exit_status_script[PROJECTILE_STATUS_MAX])();

	void projectile_main();
	void superInit();
	void load_anim_list();
	void load_stats();
	void change_anim(string animation_name, int max_ticks = 1, int entry_frame = 0);
	void startAnimation(Animation* animation);
	void loadStatusScripts();

	bool canStep();
	void stepAnimation();
	bool change_status(u32 new_status_kind, bool call_end_status = true, bool require_different_status = true);
	void playoutStatus();

	void new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset,
		float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
		int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status, 
		int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool trade, bool continue_launch);

	virtual void tickOnceProjectileUnique() {};
	virtual void projectile_unique_status() {};
	virtual void projectile_unique_enter_status() {};
	virtual void projectile_unique_exit_status() {};

	virtual void status_default();
	virtual void enter_status_default();
	virtual void exit_status_default();
	virtual void status_move();
	virtual void enter_status_move();
	virtual void exit_status_move();
	virtual void status_hit();
	virtual void enter_status_hit();
	virtual void exit_status_hit();
};