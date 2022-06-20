#pragma once
#include "BattleObject.h"
#include "ProjectileConstants.h"
#include <glm/glm.hpp>
#include <iostream>
#include "BattleObjectManager.h"

#define ADD_PROJECTILE_STATUS(index, status_func) (status_script[index] = (void (Projectile::*)(void))status_func)
#define ADD_PROJECTILE_ENTRY_STATUS(index, status_func) (enter_status_script[index] = (void (Projectile::*)(void))(status_func))
#define ADD_PROJECTILE_EXIT_STATUS(index, status_func) (exit_status_script[index] = (void (Projectile::*)(void))(status_func))

class Fighter;

class Projectile: public BattleObject {
public:
	int projectile_kind;
	std::string projectile_name;
	int owner_id;

	Fighter* owner;

	bool active = false;

	std::vector<int> projectile_int;
	std::vector<float> projectile_float;
	std::vector<bool> projectile_flag;

	std::vector<void (Projectile::*)(void)> status_script;
	std::vector<void (Projectile::*)(void)> enter_status_script;
	std::vector<void (Projectile::*)(void)> exit_status_script;

	/*
		FUNCTIONS
	*/

	//Constructors

	Projectile();
	~Projectile();

	//Main

	void projectile_main();
	virtual void projectile_unique_main() {};
	void process_status();
	void process_animate();
	void process_post_animate();
	void process_position();

	void projectile_post();
	void process_post_position();

	void decrease_common_variables();

	//Setup
	
	void super_init();
	void load_model_shader();
	void load_anim_list();
	void load_status_scripts();
	virtual void load_move_scripts() {};
	void set_default_vars();
	void init_boxes();

	void add_pos(float x, float y);
	void add_pos(glm::vec3 pos);
	void set_pos(glm::vec3 pos);

	//Animation

	void change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);

	//Status

	bool change_status(unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);

	//Hitbox

	void new_hitbox(int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, SituationHit situation_hit,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, bool trade, KoKind ko_kind, bool continue_launch, bool disable_hitstun_parry,
		float launch_init_y, float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x);

	//Script Functions
	template<typename ...T>
	void push_function(void (Projectile::* function)(ScriptArg), T... args) {
		std::queue<std::any> queue = extract_variadic_to_queue(args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.function_calls.push((void (BattleObject::*)(ScriptArg))function);
		active_script_frame.function_args.push(sa);
	}


	//Script Wrappers

	void NEW_HITBOX(ScriptArg args);

	//Status Scripts

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