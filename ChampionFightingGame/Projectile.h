#pragma once
#include "BattleObject.h"
#include "ProjectileConstants.h"
#include <glm/glm.hpp>
#include <iostream>

class FighterAccessor;

class Projectile: public BattleObject {
public:
	int projectile_kind;
	int owner_id;

	FighterAccessor* fighter_accessor;

	bool has_model;

	int projectile_int[PROJECTILE_INT_MAX];
	float projectile_float[PROJECTILE_FLOAT_MAX];
	bool projectile_flag[PROJECTILE_FLAG_MAX];

	void (Projectile::* status_script[PROJECTILE_STATUS_MAX])();
	void (Projectile::* enter_status_script[PROJECTILE_STATUS_MAX])();
	void (Projectile::* exit_status_script[PROJECTILE_STATUS_MAX])();

	/*
		FUNCTIONS
	*/

	//Main

	void projectile_main();
	virtual void projectile_unique_main() {};
	void process_status();
	void process_animate();
	void process_post_animate();
	void process_position();
	void process_post_position();

	void decrease_common_variables();

	//Setup
	
	void superInit();
	void load_model_shader();
	void load_anim_list();
	void load_status_scripts();
	void load_stats();
	void set_default_vars();
	void init_boxes();

	//Animation

	void change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);
	void startAnimation(Animation* animation);

	//Status

	bool change_status(unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);
	virtual void projectile_unique_status() {};
	virtual void projectile_unique_enter_status() {};
	virtual void projectile_unique_exit_status() {};

	//Hitbox

	void new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, glm::vec2 anchor, glm::vec2 offset,
		float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun,
		int blocklag, int blockstun, bool unblockable, float hit_pushback, float block_pushback, int juggle_set, int max_juggle, int hit_status, 
		int counterhit_status, int counterhit_type, float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, 
		bool trade, bool continue_launch, bool can_chip_ko);

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