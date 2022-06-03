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

	//Projectiles

	void init_projectile(int id, glm::vec3 pos); //Marks a projectile as active and moves it to the given position relative to the player
	void destroy_projectile(int id); //Marks a projectile as inactive

	//Setup

	void super_init(int id);
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
	int get_special_input(int special_kind, unsigned int button, int charge_frames = 0); //Checks if you're making a special input
	bool get_normal_cancel(int attack_kind, unsigned int button, int situation_kind, int stick = 10); //Attempts to cancel attack_kind into a normal based on 
		//button if the situation_kind is correct
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

	//Rotation

	void set_rot(glm::vec3 rot);
	void add_rot(glm::vec3 rot);
	void reset_rot();

	//Opponent Fighter Instance - Generally we should avoid modifying the opponent through their fighter accessor outside of these functions, or things
		//can get really hard to follow

	void change_opponent_status(unsigned int status_kind); //Wild guess.
	void damage_opponent(float damage); //Damage the opponent. 
		//Use in combination with change_opponent_status to throw someone.
	void set_opponent_rot(glm::vec3 rot); //Sets the opponent's angle relative to their facing dir.
	void add_opponent_rot(glm::vec3 rot);
	void reset_opponent_rot();
	void set_opponent_thrown_ticks(); //Sets how long the opponent should stay in an animation, might be obselete due to get_launch_ticks, not sure
	void change_opponent_anim(std::string anim_kind, float frame_rate = 1.0, float entry_frame = 0.0); //Changes the opponent's animation
	void attach_opponent(std::string bone_name);
	void detach_opponent();

	//Grab Functions
	void grab_opponent(std::string attacker_bone_name, std::string defender_bone_name, glm::vec2 offset, int frames);
	void throw_opponent(float damage, float x_speed = 0, float y_speed = 0);

	//Jostle Box

	void update_jostle_rect();
	void set_jostle_offset(float offset);

	//Hitbox
	
	void new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, glm::vec2 anchor, glm::vec2 offset,
		int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun, 
		int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind, 
		int juggle_start, int juggle_increase, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, 
		float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool can_chip_ko, bool can_ko, bool use_player_pos = true);

	//Grabbox
	
	void new_grabbox(int id, glm::vec2 anchor, glm::vec2 offset, int grabbox_kind, int situation_hit, unsigned int attacker_status_if_hit,
		unsigned int defender_status_if_hit, bool use_player_pos = true);
	
	//Hurtbox
	
	void new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, int hurtbox_kind = HURTBOX_KIND_NORMAL, bool armor = false, int intangible_kind = INTANGIBLE_KIND_NONE);

	//Transitions

	bool is_actionable();
	bool can_kara();

	//Animation
	
	void reenter_last_anim();
	bool change_anim(std::string animation_name, float rate = 1.0, float entry_frame = 0.0, bool clear_buffer = true);
	bool change_anim_inherit_attributes(std::string animation_name,  bool continue_script = true, bool clear_buffer = true, bool verbose = true);
	void startAnimation(Animation* animation, bool clear_buffer);
	bool beginning_hitlag(int frames);
	bool ending_hitlag(int frames);
	float calc_launch_frames();
	std::string get_anim();
	std::string get_anim_broad();

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
	bool is_status_end(unsigned int status_kind = FIGHTER_STATUS_WAIT, bool call_end_status = true, bool require_different_status = true);
	unsigned int get_status_group();
	bool is_status_hitstun_enable_parry();
	bool is_status_delay();

	//don't worry, it'll get longer :)
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