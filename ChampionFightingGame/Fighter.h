#pragma once
#include "BattleObject.h"
#include <string>
#include "Button.h"
#include "Animation.h"
#include "Box.h"
#include "Player.h"
#include "FighterConstants.h"
#include "BattleObjectManager.h"

#define ADD_FIGHTER_STATUS(index, status_func) (status_script[index] = (void (Fighter::*)(void))status_func)
#define ADD_FIGHTER_ENTRY_STATUS(index, status_func) (enter_status_script[index] = (void (Fighter::*)(void))(status_func))
#define ADD_FIGHTER_EXIT_STATUS(index, status_func) (exit_status_script[index] = (void (Fighter::*)(void))(status_func))

class Projectile;

class Fighter: public BattleObject {
public:
	int chara_kind;
	std::string chara_name;
	int music_kind;

	Projectile* projectiles[MAX_PROJECTILES]{};
	int num_projectiles = 0;

	int prev_stick_dir;
	
	std::vector<int> fighter_int;
	std::vector<float> fighter_float;
	std::vector<bool> fighter_flag;
	
	//Used to determine which ID of the opponent's hitboxes connected with this player. Set every frame by the collision checks, default value is -1.
	int connected_hitbox = -1;
	int connected_grabbox = -1;
	int connected_projectile_hitbox = -1;

	//Array of pointers to the corressponding function for each status
	std::vector<void (Fighter::*)(void)> status_script;
	std::vector<void (Fighter::*)(void)> enter_status_script;
	std::vector<void (Fighter::*)(void)> exit_status_script;

	//Array of flags to determine what can and cannot be canceled into

	bool cancel_flags[CANCEL_CAT_MAX][CANCEL_KIND_MAX];

	/*
		FUNCTIONS
	*/

	//Constructors

	Fighter();
	~Fighter();

	//Main

	void fighter_main(); //Runs during every frame of gameplay
	void fighter_post(); //Handles some logic which may require the opponent's main() function being finished
	//(throws requiring the opponent's bone positions being up to date, for example)

	virtual void chara_main() {}; //Runs during every frame of gameplay, specific to the character that's defining it

	void process_projectiles(); //Calls the scripts for projectiles
	void process_post_projectiles(); //Calls the post scripts for projectiles

	void process_animate(); //Increments the frame and determines whether or not the end of an animation has been reached
	void process_post_animate(); //Rotates all of the bones once animation and frame have been finalized

	void process_pre_position(); //Resets rotation as well as any garbage position values, creates a jostle rect
	void process_position(); //Checks collision, creates a jostle rect
	void process_post_position(); //Adds pushback, rotates the character based on their facing direction

	void process_input(); //Manages specific inputs such as special motions and dashes
	
	void process_pre_status(); //Executes any buffered status changes
	void process_status(); //Checks for the hitstun parry input, then runs the status and move scripts
	void process_post_status(); //Misc

	void process_ai();

	void decrease_common_variables();
	void reset();

	//Setup

	void super_init();
	void load_model_shader();
	void load_anim_list();
	void load_status_scripts();
	virtual void load_move_scripts() {};
	void load_fighter_sounds();
	virtual void load_chara_sounds() {};
	void load_fighter_effects();
	virtual void load_chara_effects() {};
	void set_default_vars();
	void init_boxes();

	//Inputs

	bool check_button_on(unsigned int button); //Checks if a button is being pressed
	bool check_button_input(unsigned int button); //Checks if a button was pressed within the buffer window
	bool check_button_input(unsigned int buttons[], int length, int min_matches = 0); //Same as above but for multiple buttons, returning true if at least 
		//min_matches buttons were pressed
	bool check_button_trigger(unsigned int button); //Checks if a button was pressed on that frame
	bool check_button_release(unsigned int button); //Checks if a button was released on that frame
	int get_stick_dir(bool internal_dir = true); //Stick direction, relative to your facing direction. Returns num pad notation.
	int get_flick_dir(bool internal_dir = true); //Same as above, but returns 0 if your direction didn't change on that frame
	bool get_normal_input(int attack_kind, unsigned int button = 0, int stick_dir = 0);
	int get_special_input(int special_kind, unsigned int button, int charge_frames = 0); //Checks if you're making a special input
	bool normal_cancel(int attack_kind, unsigned int button = 0, int stick_dir = 10);
	int try_ex(bool punch); //Checks if you had enough meter to use an EX special. If you did, done. If you didn't, check whether or not one of your
		//buttons in the EX input were Heavy. If so, use a Heavy special, otherwise use a Medium special.

	//Param Helper Funcs - Call the normal get_param functions but will append the move strength of the special you're in

	int get_param_int_special(std::string param);
	float get_param_float_special(std::string param);
	bool get_param_bool_special(std::string param);
	std::string get_param_string_special(std::string param);

	//Position - For both of these functions, the "prev" arg determines what to do if the position is invalid. If prev is true, you'll go to the last
		//position on each coordinate, so an invalid x but valid y will only modify your x. If prev is false, you'll go to the closest valid position
		//to where you want to go to, so if your x would be higher than the window bounds, your x position would be set to the window bounds.

	bool add_pos(glm::vec3 pos, bool prev = false);
	bool add_pos(float x, float y, float z = 0.0, bool prev = false);
	bool set_pos(glm::vec3 pos, bool prev = false);
	bool set_pos(float x, float y, float z = 0.0, bool prev = false);
	bool set_pos_anim();
	void landing_crossup();
	void apply_gravity(float gravity, float fall_speed);

	//Rotation

	void set_rot(glm::vec3 rot);
	void add_rot(glm::vec3 rot);
	void reset_rot();

	//Data

	void set_int(int target, int val);
	void inc_int(int target);
	void dec_int(int target);
	void set_float(int target, float val);
	void set_flag(int target, bool val);

	//Hitbox
	
	void new_hitbox(int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, SituationHit situation_hit,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, ClankKind clank_kind, KoKind ko_kind, bool continue_launch,
		bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
		float launch_max_fall_speed, float launch_speed_x, bool use_player_pos
	);

	//Grabbox
	
	void new_grabbox(int id, glm::vec2 anchor, glm::vec2 offset, int grabbox_kind, int situation_hit, unsigned int attacker_status_if_hit,
		unsigned int defender_status_if_hit, bool use_player_pos = true);
	
	//Hurtbox
	
	void new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, int hurtbox_kind = HURTBOX_KIND_NORMAL, bool armor = false, int intangible_kind = INTANGIBLE_KIND_NONE);

	//Grab Functions

	void grab_opponent(std::string attacker_bone_name, std::string defender_bone_name, glm::vec2 offset, int frames);
	void throw_opponent(float damage, float x_speed = 0, float y_speed = 0);

	//Jostle Box

	void update_jostle_rect();
	void set_jostle_offset(float offset);

	//Animation
	
	void reenter_last_anim();
	bool change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0);
	bool change_anim_inherit_attributes(std::string animation_name,  bool continue_script = true, bool verbose = true);
	bool beginning_hitlag(int frames);
	bool ending_hitlag(int frames);
	float calc_launch_frames();
	std::string get_anim();
	std::string get_anim_broad();

	//Actionability

	int get_frames_until_actionable();
	bool is_actionable();
	bool can_kara();
	bool has_meter(int bars);
	void enable_all_cancels();
	void enable_cancel(int cat, int kind);
	void disable_all_cancels();
	void disable_cancel(int cat, int kind);
	bool is_enable_cancel(int cat, int kind);

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

	bool change_status(unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);
	bool change_status_after_hitlag(unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);
	virtual void chara_status() {};
	virtual void chara_enter_status() {};
	virtual void chara_exit_status() {};
	bool common_ground_status_act(bool crouch = true);
	bool common_air_status_act();
	bool common_air_status_general();
	virtual bool specific_ground_status_act() { return false; };
	virtual bool specific_air_status_act() { return false; };
	virtual bool specific_status_attack() { return false; };
	bool is_status_end(unsigned int post_status_kind = FIGHTER_STATUS_WAIT, bool call_end_status = true, bool require_different_status = true);
	bool check_landing(unsigned int post_status_kind = FIGHTER_STATUS_LANDING, bool call_end_status = true, bool require_different_status = true);
	unsigned int get_status_group();
	bool check_hitstun_parry();
	bool is_status_delay();

	//Projectiles

	void activate_projectile(int id, glm::vec3 pos); //Marks a projectile as active and moves it to the given position relative to the player
	void deactivate_projectile(int id); //Marks a projectile as inactive
	void set_projectile_int(int projectile, int target, int val);
	void set_projectile_float(int projectile, int target, float val);
	void set_projectile_flag(int projectile, int target, bool val);
	void add_projectile_pos(int projectile, int pos_x, int pos_y);
	void add_projectile_pos(int projectile, glm::vec3 pos);
	void set_projectile_pos(int projectile, int pos_x, int pos_y);
	void set_projectile_pos(int projectile, glm::vec3 pos);
	void change_projectile_status(int projectile, unsigned int new_status_kind, bool call_end_status = true, bool require_different_status = true);

	//Opponent

	void change_opponent_status(unsigned int status_kind); //Wild guess.
	void damage_opponent(float damage); //Damage the opponent.
	void set_opponent_rot(glm::vec3 rot); //Sets the opponent's angle relative to their facing dir.
	void add_opponent_rot(glm::vec3 rot);
	void reset_opponent_rot();
	void set_opponent_thrown_ticks(); //Sets how long the opponent should stay in an animation, might be obselete due to get_launch_ticks, not sure
	void change_opponent_anim(std::string anim_kind, float frame_rate = 1.0, float entry_frame = 0.0); //Changes the opponent's animation
	void attach_opponent(std::string bone_name);
	void detach_opponent();

	//Script Functions
	template<typename ...T>
	void push_function(void (Fighter::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.function_calls.push((void (BattleObject::*)(ScriptArg))function);
		active_script_frame.function_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	//Script Wrappers

	void SET_INT(ScriptArg args);
	void SET_FLOAT(ScriptArg args);
	void SET_FLAG(ScriptArg args);

	void NEW_HITBOX(ScriptArg args);
	void CLEAR_HITBOX(ScriptArg args);
	void CLEAR_HITBOX_ALL(ScriptArg args);

	void NEW_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX_ALL(ScriptArg args);

	void NEW_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX_ALL(ScriptArg args);

	void GRAB_OPPONENT(ScriptArg args);
	void THROW_OPPONENT(ScriptArg args);

	void SET_JOSTLE_OFFSET(ScriptArg args);

	void ADD_POS(ScriptArg args);
	void SET_POS(ScriptArg args);

	void REENTER_LAST_ANIM(ScriptArg args);

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
	void CHANGE_PROJECTILE_STATUS(ScriptArg args);

	void CHANGE_OPPONENT_STATUS(ScriptArg args);
	void CHANGE_OPPONENT_ANIM(ScriptArg args);

	//Status Scripts

	virtual void status_wait();
	virtual void enter_status_wait();
	virtual void exit_status_wait();
	virtual void status_walkf();
	virtual void enter_status_walkf();
	virtual void exit_status_walkf();
	virtual void status_walkb();
	virtual void enter_status_walkb();
	virtual void exit_status_walkb();
	virtual void status_dash();
	virtual void enter_status_dash();
	virtual void exit_status_dash();
	virtual void status_dashb();
	virtual void enter_status_dashb();
	virtual void exit_status_dashb();
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
	virtual void status_advance();
	virtual void enter_status_advance();
	virtual void exit_status_advance();
	virtual void status_advance_forward();
	virtual void enter_status_advance_forward();
	virtual void exit_status_advance_forward();
	virtual void status_advance_back();
	virtual void enter_status_advance_back();
	virtual void exit_status_advance_back();
	virtual void status_grab();
	virtual void enter_status_grab();
	virtual void exit_status_grab();
	virtual void status_throw();
	virtual void enter_status_throw();
	virtual void exit_status_throw();
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
	virtual void status_hitstun();
	virtual void enter_status_hitstun();
	virtual void exit_status_hitstun();
	virtual void status_hitstun_air();
	virtual void enter_status_hitstun_air();
	virtual void exit_status_hitstun_air();
	virtual void status_blockstun();
	virtual void enter_status_blockstun();
	virtual void exit_status_blockstun();
	virtual void status_parry_start();
	virtual void enter_status_parry_start();
	virtual void exit_status_parry_start();
	virtual void status_parry();
	virtual void enter_status_parry();
	virtual void exit_status_parry();
	virtual void status_crumple();
	virtual void enter_status_crumple();
	virtual void exit_status_crumple();
	virtual void status_launch_start();
	virtual void enter_status_launch_start();
	virtual void exit_status_launch_start();
	virtual void status_launch();
	virtual void enter_status_launch();
	virtual void exit_status_launch();
	virtual void status_clank();
	virtual void enter_status_clank();
	virtual void exit_status_clank();
	virtual void status_throw_tech();
	virtual void enter_status_throw_tech();
	virtual void exit_status_throw_tech();
	virtual void status_landing();
	virtual void enter_status_landing();
	virtual void exit_status_landing();
	virtual void status_landing_attack();
	virtual void enter_status_landing_attack();
	virtual void exit_status_landing_attack();
	virtual void status_landing_hitstun();
	virtual void enter_status_landing_hitstun();
	virtual void exit_status_landing_hitstun();
	virtual void status_knockdown_start();
	virtual void enter_status_knockdown_start();
	virtual void exit_status_knockdown_start();
	virtual void status_knockdown();
	virtual void enter_status_knockdown();
	virtual void exit_status_knockdown();
	virtual void status_wakeup();
	virtual void enter_status_wakeup();
	virtual void exit_status_wakeup();

	virtual void chara_template_status_template() {};
	virtual void chara_template_enter_status_template() {};
	virtual void chara_template_exit_status_template() {};
};