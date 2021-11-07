#pragma once
#include "Object.h"
#include "Projectile.h"
#include "utils.h"
#include <string>
#include "GameCoordinate.h"
#include "Button.h"
#include <SDL.h>
#include "Animation.h"
#include "ParamTable.h"
#include "Fighter.fwd.h"
#include "Box.fwd.h"
#include "Box.h"
#include "FighterAccessor.fwd.h"
#include "FighterAccessor.h"
#include "PlayerInfo.h"
#include "Game.fwd.h"
#include "Game.h"

class Fighter: public Object {
public:
	//Haha interface go brrrrrrrrrrrr

	virtual void chara_id() = 0;

	//Most variables are handled by the Fighter arrays, but if something is primarily used outside of the Fighter class, it is a direct member

	//Misc important data

	int chara_kind;
	FighterAccessor* fighter_accessor;
	Projectile* projectiles[MAX_PROJECTILES]{}; //The actual Projectile class
	IObject* projectile_objects[MAX_PROJECTILES]{}; //Used to instantiate Projectiles of different child types 
	bool requesting_priority = false; //Checked by the fighter_accessor to determine which render priority value to use
	bool crash_to_debug{ false };
	int prev_stick_dir;
	
	int fighter_int[FIGHTER_INT_MAX]{ 0 };
	float fighter_float[FIGHTER_FLOAT_MAX]{ 0.0 };
	bool fighter_flag[FIGHTER_FLAG_MAX]{false};
	bool kara_enabled{ false }; //Should be made into a Fighter Flag tbh, never used outside of status scripts
	
	//Used to determine which ID of the opponent's hitboxes connected with this player. Set every frame by the collision checks, default value is -1.
	int connected_hitbox = -1;
	int connected_grabbox = -1;
	int connected_projectile_hitbox = -1;

	//Array of pointers to the corressponding function for each status
	void (Fighter::* status_script[FIGHTER_STATUS_MAX])(); //Runs every frame for the status you're in
	void (Fighter::* enter_status_script[FIGHTER_STATUS_MAX])(); //While your status is being changed, runs for the status you're going into
	void (Fighter::* exit_status_script[FIGHTER_STATUS_MAX])(); //While your status is being changed, runs for the status you're going out of

	/*
		FUNCTIONS
	*/

	//Constructors

	Fighter();
	Fighter(PlayerInfo *player_info);

	void fighter_main(); //Runs during every frame of gameplay
	virtual void chara_main() {}; //Runs during every frame of gameplay, specific to the character that's defining it
	void create_jostle_rect(GameCoordinate anchor, GameCoordinate offset); //Sets up the player's jostle box, called multiple times every frame

	//Projectiles
	void init_projectile(int id, GameCoordinate pos); //Marks a projectile as active and moves it to the given position relative to the player
	void destroy_projectile(int id); //Marks a projectile as inactive

	//Setup

	void superInit(int id);
	void load_anim_list();
	void load_anim_map(SDL_Rect ret[MAX_ANIM_LENGTH], string anim_dir);
	void loadStatusScripts();

	//Inputs

	void processInput(); //Manages specific inputs such as special motions and dashes
	bool check_button_on(u32 button); //Checks if a button is being pressed
	bool check_button_input(u32 button); //Checks if a button was pressed within the buffer window
	bool check_button_input(u32 buttons[], int length, int min_matches = 0); //Same as above but for multiple buttons, returning true if at least min_matches buttons were pressed
	bool check_button_trigger(u32 button); //Checks if a button was pressed on that frame
	bool check_button_release(u32 button); //Checks if a button was released on that frame
	int get_stick_dir(); //Stick direction, relative to your facing direction. Returns num pad notation.
	int get_flick_dir(); //Same as above, but returns 0 if your direction didn't change on that frame
	int get_special_input(int special_kind, u32 button, int charge_frames = 0); //Checks if you're making a special input
	bool get_normal_cancel(int attack_kind, u32 button, int situation_kind); //Attempts to cancel attack_kind into a normal based on button if the situation_kind is correct

	//Param Helper Funcs - Call the normal get_param functions but will append the move strength of the 

	int get_param_int_special(string param);
	float get_param_float_special(string param);
	bool get_param_bool_special(string param);
	string get_param_string_special(string param);

	//Position

	bool add_pos(float x, float y, bool prev = false);
	bool set_pos(float x, float y, bool prev = false);

	//Messing with the opponent's Fighter

	void set_opponent_offset(GameCoordinate offset, int frames);
	void set_opponent_offset(GameCoordinate offset);
	void change_opponent_status(u32 status_kind);
	void damage_opponent(float damage, float facing_dir, float x_speed = 0, float y_speed = 0);
	void set_opponent_angle(double angle);
	void set_opponent_thrown_ticks();
	void change_opponent_anim(string anim_kind, int frame_rate = 1, int entry_frame = 0);

	//Hitbox
	
	void new_hitbox(int id, int multihit, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset, 
		int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun, 
		int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind, 
		bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, 
		float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool continue_launch, bool use_player_pos = true);
	
	//Grabbox
	
	void new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit, 
		u32 defender_status_if_hit, bool use_player_pos = true);
	
	//Hurtbox
	
	void new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind = HURTBOX_KIND_NORMAL, bool armor = false, int intangible_kind = INTANGIBLE_KIND_NONE);

	//Transitions

	bool is_actionable();
	bool can_kara();

	//Animation
	
	void reenter_last_anim();
	bool change_anim(string animation_name, int max_ticks = 1, int entry_frame = 0);
	bool change_anim_inherit_attributes(string animation_name, bool verbose = true,  bool continue_script = true);
	void startAnimation(Animation* animation);
	bool canStep();
	void stepAnimation();
	void forceStepThroughHitlag();
	bool beginning_hitlag(int frames);
	bool ending_hitlag(int frames);
	int get_launch_ticks();
	string get_anim();
	string get_anim_broad();

	//Status

	bool change_status(u32 new_status_kind, bool call_end_status = true, bool require_different_status = true);
	bool change_status_after_hitlag(u32 new_status_kind, bool call_end_status = true, bool require_different_status = true);
	void playoutStatus();
	virtual void chara_status() {};
	virtual void chara_enter_status() {};
	virtual void chara_exit_status() {};
	bool common_ground_status_act();
	bool common_air_status_act();
	bool common_air_status_general();
	virtual bool specific_ground_status_act() { return false; };
	virtual bool specific_air_status_act() { return false; };
	virtual bool specific_status_attack() { return false; };
	bool is_status_end(u32 status_kind = FIGHTER_STATUS_WAIT, bool call_end_status = true, bool require_different_status = true);
	u32 get_status_group();
	bool is_status_hitstun_enable_parry();

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