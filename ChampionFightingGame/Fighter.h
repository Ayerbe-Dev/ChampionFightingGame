#pragma once
#include "BattleObject.h"
#include <string>
#include "Button.h"
#include "Animation.h"
#include "Player.h"
#include "ObjectManager.h"
#include "ParamAccessor.h"
#include "FighterMovelist.h"

#include "CharaKind.h"
#include "FighterAttribute.h"
#include "FighterInt.h"
#include "FighterFlag.h"
#include "FighterFloat.h"
#include "FighterString.h"
#include "FighterStatus.h"

#define ADD_FIGHTER_STATUS(index, status_func) (status_script[index] = (void (Fighter::*)(void))status_func)
#define ADD_FIGHTER_ENTRY_STATUS(index, status_func) (enter_status_script[index] = (void (Fighter::*)(void))(status_func))
#define ADD_FIGHTER_EXIT_STATUS(index, status_func) (exit_status_script[index] = (void (Fighter::*)(void))(status_func))

class Fighter: public BattleObject {
public:
	Fighter();
	~Fighter();

	/*
		FIGHTER FUNCTIONS
	*/

	//Main

	void fighter_main(); //Runs during every frame of gameplay
	void fighter_post(); //Handles some logic which may require the opponent's main() function being finished
	//(throws requiring the opponent's bone positions being up to date, for example)

	virtual void chara_main() {}; //Runs during every frame of gameplay, specific to the character that's defining it

	void process_projectiles(); //Calls the scripts for projectiles
	void process_post_projectiles(); //Calls the post scripts for projectiles

	void process_animate(); //Increments the frame and determines whether or not the end of an animation has been reached
	void process_post_animate(); //Rotates all of the bones once animation and frame have been finalized

	void process_position(); //Resets rotation as well as any garbage position values
	void process_post_position(); //Adds pushback, rotates the character based on their facing direction

	void process_input(); //Manages specific inputs such as special motions and dashes
	
	void process_pre_status(); //Executes any buffered status changes
	void process_status(); //Checks for the hitstun parry input, then runs the status and move scripts
	void process_post_status(); //Misc

	void decrease_common_variables();
	void reset();

	//Loading

	void load_fighter();
	void load_model_shader();
	void load_anim_list();
	void load_fighter_status_scripts();
	virtual void load_chara_status_scripts() {};
	virtual void load_move_scripts() {};
	virtual void load_move_list() {};
	void load_sound_list();
	void load_effect_list();
	virtual void load_chara_effects() {};
	void set_default_vars();
	void load_collision_boxes();

	//Inputs

	bool check_button_on(unsigned int button); //Checks if a button is being pressed
	bool check_button_input(unsigned int button); //Checks if a button was pressed within the buffer window
	bool check_button_input(unsigned int buttons[], int length, int min_matches = 0); //Same as above but for multiple buttons, returning true if at least 
		//min_matches buttons were pressed
	bool check_button_trigger(unsigned int button); //Checks if a button was pressed on that frame
	bool check_button_release(unsigned int button); //Checks if a button was released on that frame
	unsigned int get_stick_dir(bool internal_dir = true); //Stick direction, relative to your facing direction. Returns num pad notation.
	unsigned int get_stick_dir_no_lr();
	unsigned int get_flick_dir(bool internal_dir = true); //Same as above, but returns 0 if your direction didn't change on that frame
	unsigned int get_buffer_stick_dir(bool internal_dir = true); //Reads from the buffer code instead of the current frame
	
	//Param Helper Funcs - Call the normal get_param functions but will append the move strength of the special you're in

	int get_param_int_special(std::string param);
	float get_param_float_special(std::string param);
	bool get_param_bool_special(std::string param);
	std::string get_param_string_special(std::string param);

	//Position - For both of these functions, the "prev" arg determines what to do if the position is invalid. If prev is true, you'll go to the last
		//position on each coordinate, so an invalid x but valid y will only modify your x. If prev is false, you'll go to the closest valid position
		//to where you want to go to, so if your x would be higher than the window bounds, your x position would be set to the window bounds.

	bool add_pos(glm::vec3 pos, bool prev = false);
	bool set_pos(glm::vec3 pos, bool prev = false);
	bool set_pos_anim();
	void landing_crossup();
	void apply_gravity(float gravity, float fall_speed);

	//Data

	void set_int(int target, int val);
	void inc_int(int target);
	void dec_int(int target);
	void set_float(int target, float val);
	void set_flag(int target, bool val);
	void set_string(int target, std::string val);

	bool can_spend_ex(float ex);
	void spend_ex(float ex);
	void gain_ex(float ex);

	//Animation
	
	bool change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);
	bool change_anim_inherit_attributes(std::string animation_name,  bool continue_script = true, bool verbose = true);
	bool beginning_hitlag(int frames);
	bool ending_hitlag(int frames);
	float calc_launch_frames();

	//Actions

	bool is_actionable();
	bool is_enable_free_cancel();
	void enable_free_cancel();
	void enable_cancel(std::string move_kind, CancelKind cancel_kind);
	void disable_all_cancels();
	void disable_cancel(std::string move_kind, CancelKind cancel_kind);
	bool is_enable_cancel(std::string move_kind);
	FighterMoveListEntry get_curr_move();
	void check_movelist_inputs();

	//Frame Data

	int get_frames_until_actionable();

	//Cinematic

	void start_cinematic_sequence(std::string anim_kind, float anim_rate, float anim_frame, float world_brightness, bool dim_self, float world_rate);
	void stop_cinematic_sequence();

	void play_camera_anim(std::string anim_kind, float rate, float frame);
	void stop_camera_anim();

	void slow_world(float world_rate);
	void reset_world_rate();

	void dim_world(float world_brightness, bool dim_self);
	void reset_world_brightness();

	//Status

	void change_status(unsigned int new_status_kind, bool call_end_status = true) override;
	void change_situation(unsigned int new_situation_kind);
	virtual void chara_status() {};
	virtual void chara_enter_status() {};
	virtual void chara_exit_status() {};
	bool common_ground_status_act();
	void common_air_status_general();
	virtual bool chara_ground_status_act() { return false; };
	virtual bool chara_status_attack() { return false; };
	virtual bool chara_status_attack_air() { return false; };
	bool is_status_end(unsigned int post_status_kind = FIGHTER_STATUS_NONE, bool call_end_status = true, bool require_different_status = true);
	bool check_landing(unsigned int post_status_kind = FIGHTER_STATUS_LANDING, bool call_end_status = true);
	bool check_hitstun_parry();
	bool is_ko();

	//Projectiles

	void activate_projectile(int id, glm::vec3 pos); //Marks a projectile as active and moves it to the given position relative to the player
	void deactivate_projectile(int id); //Marks a projectile as inactive
	void set_projectile_int(int projectile, int target, int val);
	void set_projectile_float(int projectile, int target, float val);
	void set_projectile_flag(int projectile, int target, bool val);
	void set_projectile_string(int projectile, int target, std::string val);
	void add_projectile_pos(int projectile, glm::vec3 pos);
	void set_projectile_pos(int projectile, glm::vec3 pos);
	void change_projectile_status(int projectile, unsigned int new_status_kind, bool call_end_status = true);

	//Opponent

	void change_opponent_status(unsigned int status_kind); //Wild guess.
	void damage_opponent(float damage); //Damage the opponent.
	void set_opponent_thrown_ticks(); //Sets how long the opponent should stay in an animation, might be obselete due to get_launch_ticks, not sure
	void change_opponent_anim(std::string anim_kind, float frame_rate = 1.0, float entry_frame = 0.0); //Changes the opponent's animation
	void grab_opponent(std::string attacker_bone_name, std::string defender_bone_name, glm::vec3 offset);


	//Collision Functions

	void process_fighter_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes);
	void process_projectile_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes);
	bool is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) override;
	bool is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) override;

	void process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) override;
	void process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) override;
	void process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) override;
	void process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) override;
	void process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) override;
	void process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) override;

	virtual void unique_process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker){};
	virtual void unique_process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker){};
	virtual void unique_process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender){};
	virtual void unique_process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender){};

	void check_incoming_blockbox_collisions(std::vector<Blockbox*> blockboxes);

	void check_incoming_grabbox_collisions(std::vector<Grabbox*> grabboxes);
	bool is_valid_incoming_grabbox_collision(Hurtbox* hurtbox, Grabbox* grabbox);

	void process_incoming_grabbox_collision(Grabbox* grabbox, BattleObject* attacker);
	void process_incoming_fighter_grabbox_collision(Grabbox* grabbox, Fighter* attacker);
	void process_incoming_projectile_grabbox_collision(Grabbox* grabbox, Projectile* attacker);
	void process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender);

	virtual void unique_process_incoming_grabbox_collision(Grabbox* grabbox, BattleObject* attacker) {};
	virtual void unique_process_incoming_fighter_grabbox_collision(Grabbox* grabbox, Fighter* attacker) {};
	virtual void unique_process_incoming_projectile_grabbox_collision(Grabbox* grabbox, Projectile* attacker) {};
	virtual void unique_process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {};

	void process_definite_hitbox_activated(DefiniteHitbox* hitbox, Fighter* attacker);

	int get_counterhit_val(Hitbox* hitbox);
	void set_post_collision_status(Hitbox* hitbox, int counterhit_val);

	//Script Functions
	template<typename ...T>
	void push_function(void (Fighter::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_function((void (BattleObject::*)(ScriptArg))function, sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_true(std::string condition_name, void (Fighter::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_true(condition_name, (void (BattleObject::*)(ScriptArg))function, sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_false(std::string condition_name, void (Fighter::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_false(condition_name, (void (BattleObject::*)(ScriptArg))function, sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	//Script Wrappers

	void SET_INT(ScriptArg args);
	void SET_FLOAT(ScriptArg args);
	void SET_FLAG(ScriptArg args);
	void SET_STRING(ScriptArg args);

	void ADD_POS(ScriptArg args);
	void SET_POS(ScriptArg args);

	void CHANGE_ANIM(ScriptArg args);

	void START_CINEMATIC_SEQUENCE(ScriptArg args);
	void RESET_WORLD_RATE(ScriptArg args);

	void ENABLE_CANCEL(ScriptArg args);
	void DISABLE_CANCEL(ScriptArg args);
	void DISABLE_ALL_CANCELS(ScriptArg args);

	void CHANGE_STATUS(ScriptArg args);

	void ACTIVATE_PROJECTILE(ScriptArg args);
	void DEACTIVATE_PROJECTILE(ScriptArg args);
	void ADD_PROJECTILE_POS(ScriptArg args);
	void SET_PROJECTILE_POS(ScriptArg args);
	void SET_PROJECTILE_INT(ScriptArg args);
	void SET_PROJECTILE_FLOAT(ScriptArg args);
	void SET_PROJECTILE_FLAG(ScriptArg args);
	void SET_PROJECTILE_STRING(ScriptArg args);
	void CHANGE_PROJECTILE_STATUS(ScriptArg args);

	void CHANGE_OPPONENT_STATUS(ScriptArg args);
	void CHANGE_OPPONENT_ANIM(ScriptArg args);
	void GRAB_OPPONENT(ScriptArg args);

	//Status Scripts

	virtual void status_none();
	virtual void enter_status_none();
	virtual void exit_status_none();
	virtual void status_wait();
	virtual void enter_status_wait();
	virtual void exit_status_wait();
	virtual void status_walk_f();
	virtual void enter_status_walk_f();
	virtual void exit_status_walk_f();
	virtual void status_walk_b();
	virtual void enter_status_walk_b();
	virtual void exit_status_walk_b();
	virtual void status_dash_f();
	virtual void enter_status_dash_f();
	virtual void exit_status_dash_f();
	virtual void status_dash_b();
	virtual void enter_status_dash_b();
	virtual void exit_status_dash_b();
	virtual void status_dash_air();
	virtual void enter_status_dash_air();
	virtual void exit_status_dash_air();
	virtual void status_crouchd();
	virtual void enter_status_crouchd();
	virtual void exit_status_crouchd();
	virtual void status_crouch();
	virtual void enter_status_crouch();
	virtual void exit_status_crouch();
	virtual void status_crouchu();
	virtual void enter_status_crouchu();
	virtual void exit_status_crouchu();
	virtual void status_jumpsquat();
	virtual void enter_status_jumpsquat();
	virtual void exit_status_jumpsquat();
	virtual void status_jump();
	virtual void enter_status_jump();
	virtual void exit_status_jump();
	virtual void status_fall();
	virtual void enter_status_fall();
	virtual void exit_status_fall();
	virtual void status_turn();
	virtual void enter_status_turn();
	virtual void exit_status_turn();
	virtual void status_attack();
	virtual void enter_status_attack();
	virtual void exit_status_attack();
	virtual void status_attack_air();
	virtual void enter_status_attack_air();
	virtual void exit_status_attack_air();
	virtual void status_backdash_attack();
	virtual void enter_status_backdash_attack();
	virtual void exit_status_backdash_attack();
	virtual void status_advance();
	virtual void enter_status_advance();
	virtual void exit_status_advance();
	virtual void status_advance_forward();
	virtual void enter_status_advance_forward();
	virtual void exit_status_advance_forward();
	virtual void status_advance_back();
	virtual void enter_status_advance_back();
	virtual void exit_status_advance_back();
	virtual void status_grab_start();
	virtual void enter_status_grab_start();
	virtual void exit_status_grab_start();
	virtual void status_grab();
	virtual void enter_status_grab();
	virtual void exit_status_grab();
	virtual void status_throw();
	virtual void enter_status_throw();
	virtual void exit_status_throw();
	virtual void status_grab_air_start();
	virtual void enter_status_grab_air_start();
	virtual void exit_status_grab_air_start();
	virtual void status_grab_air();
	virtual void enter_status_grab_air();
	virtual void exit_status_grab_air();
	virtual void status_throw_air();
	virtual void enter_status_throw_air();
	virtual void exit_status_throw_air();
	virtual void status_grabbed();
	virtual void enter_status_grabbed();
	virtual void exit_status_grabbed();
	virtual void status_thrown();
	virtual void enter_status_thrown();
	virtual void exit_status_thrown();
	virtual void status_throw_tech();
	virtual void enter_status_throw_tech();
	virtual void exit_status_throw_tech();
	virtual void status_blockstun();
	virtual void enter_status_blockstun();
	virtual void exit_status_blockstun();
	virtual void status_hitstun();
	virtual void enter_status_hitstun();
	virtual void exit_status_hitstun();
	virtual void status_hitstun_air();
	virtual void enter_status_hitstun_air();
	virtual void exit_status_hitstun_air();
	virtual void status_hitstun_float();
	virtual void enter_status_hitstun_float();
	virtual void exit_status_hitstun_float();
	virtual void status_launch_start();
	virtual void enter_status_launch_start();
	virtual void exit_status_launch_start();
	virtual void status_launch();
	virtual void enter_status_launch();
	virtual void exit_status_launch();
	virtual void status_landing();
	virtual void enter_status_landing();
	virtual void exit_status_landing();
	virtual void status_crumple();
	virtual void enter_status_crumple();
	virtual void exit_status_crumple();
	virtual void status_knockdown_start();
	virtual void enter_status_knockdown_start();
	virtual void exit_status_knockdown_start();
	virtual void status_knockdown();
	virtual void enter_status_knockdown();
	virtual void exit_status_knockdown();
	virtual void status_wakeup();
	virtual void enter_status_wakeup();
	virtual void exit_status_wakeup();
	virtual void status_parry_start();
	virtual void enter_status_parry_start();
	virtual void exit_status_parry_start();
	virtual void status_hitstun_parry_start();
	virtual void enter_status_hitstun_parry_start();
	virtual void exit_status_hitstun_parry_start();
	virtual void status_launch_parry_start();
	virtual void enter_status_launch_parry_start();
	virtual void exit_status_launch_parry_start();
	virtual void status_parry();
	virtual void enter_status_parry();
	virtual void exit_status_parry();
	virtual void status_landing_attack();
	virtual void enter_status_landing_attack();
	virtual void exit_status_landing_attack();
	virtual void status_landing_hitstun();
	virtual void enter_status_landing_hitstun();
	virtual void exit_status_landing_hitstun();
	virtual void status_taunt();
	virtual void enter_status_taunt();
	virtual void exit_status_taunt();
	virtual void status_ko();
	virtual void enter_status_ko();
	virtual void exit_status_ko();
	virtual void status_round_end();
	virtual void enter_status_round_end();
	virtual void exit_status_round_end();

	virtual void chara_template_status_template() {};
	virtual void chara_template_enter_status_template() {};
	virtual void chara_template_exit_status_template() {};

	int chara_kind;
	std::string chara_name;
	std::vector<Projectile*> projectiles;

	int prev_stick_dir;

	unsigned int situation_kind;

	std::vector<int> fighter_int;
	std::vector<float> fighter_float;
	std::vector<bool> fighter_flag;
	std::vector<std::string> fighter_string;

	//Array of pointers to the corressponding function for each status
	std::vector<void (Fighter::*)(void)> status_script;
	std::vector<void (Fighter::*)(void)> enter_status_script;
	std::vector<void (Fighter::*)(void)> exit_status_script;

	FighterMoveList move_list[FIGHTER_SITUATION_MAX];

	std::map<unsigned int, std::string> throw_map_ground;
	std::map<unsigned int, std::string> throw_map_air;
};