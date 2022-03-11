#pragma once
#include "BattleObject.h"
#include "ProjectileConstants.h"
#include <glm/glm.hpp>
#include <iostream>

#define ADD_PROJECTILE_STATUS(index, status_func) (status_script[index] = (void (Projectile::*)(void))status_func)
#define ADD_PROJECTILE_ENTRY_STATUS(index, status_func) (enter_status_script[index] = (void (Projectile::*)(void))(status_func))
#define ADD_PROJECTILE_EXIT_STATUS(index, status_func) (exit_status_script[index] = (void (Projectile::*)(void))(status_func))

class Fighter;
class FighterAccessor;

class Projectile: public BattleObject {
public:
	int projectile_kind;
	std::string projectile_name;
	int owner_id;

	FighterAccessor* fighter_accessor;
	Fighter* owner;

	bool has_model;

	std::vector<int> projectile_int;
	std::vector<float> projectile_float;
	std::vector<bool> projectile_flag;

	std::vector<void (Projectile::*)(void)> status_script;
	std::vector<void (Projectile::*)(void)> enter_status_script;
	std::vector<void (Projectile::*)(void)> exit_status_script;

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
	void set_default_vars();
	void init_boxes();

	//Animation

	void change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);
	void startAnimation(Animation* animation);

	//Status

	bool change_status(unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);

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