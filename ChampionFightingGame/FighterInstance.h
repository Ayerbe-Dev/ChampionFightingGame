#pragma once
#include "utils.h"
#include <string>
#include "GameCoordinate.h"
#include "Button.h"
#include <SDL.h>
#include "Animation.h"
#include "ParamTable.h"
#include "FighterInstance.fwd.h"
#include "Box.fwd.h"
#include "Box.h"
#include "FighterInstanceAccessor.fwd.h"
#include "FighterInstanceAccessor.h"
#include "PlayerInfo.h"

class FighterInstance{
public:
	i64 id;
	int chara_kind;

	PlayerInfo* player_info;
	GameCoordinate pos;
	GameCoordinate prevpos;
	bool facing_right{ true };
	float facing_dir{ 1.0 };
	FighterInstanceAccessor *fighter_instance_accessor;

	u32 status_kind{ CHARA_STATUS_WAIT };
	u32 situation_kind{ CHARA_SITUATION_GROUND };
	
	Animation* anim_kind;
	Animation animation_table[256];
	int frame;
	int render_frame;
	int last_excute_frame;
	int excute_count;
	u32 hold_ms;
	u32 last_frame_ms;
	bool is_anim_end{ false };
	bool kara_enabled{ false };
	
	int prev_stick_dir;
	
	StatsTable stats;
	int chara_int[CHARA_INT_MAX];
	float chara_float[CHARA_FLOAT_MAX];
	bool chara_flag[CHARA_FLAG_MAX];
	
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];
	int connected_hitbox;
	int connected_grabbox;
	
	string resource_dir;
	SDL_Texture* base_texture;
	SDL_Rect base_rect;
	SDL_Rect frame_rect;

	void (FighterInstance::* pStatus[CHARA_STATUS_MAX])();
	void (FighterInstance::* pEnter_status[CHARA_STATUS_MAX])();
	void (FighterInstance::* pExit_status[CHARA_STATUS_MAX])();
	function<void()> move_script;

	//Haha interface go brrrrrrrrrrrr

	virtual void chara_id() = 0;

	//Constructors

	FighterInstance();
	FighterInstance(SDL_Renderer* renderer, PlayerInfo *player_info);

	//Setup

	void superInit(int id, SDL_Renderer* renderer);
	void load_anim_list(SDL_Renderer* renderer);
	void load_params();
	void loadStatusFunctions();

	//Definitely not ACMD

	virtual void set_current_move_script(string anim_name);
	bool is_excute_frame(int excute_count, int frame);
	bool is_excute_wait(int excute_count, int frames);

	//Inputs

	void processInput();
	bool check_button_on(u32 button);
	bool check_button_input(u32 button, u32 button_2nd = BUTTON_MAX);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	int get_stick_dir();
	int get_flick_dir();
	int get_special_input(int special_kind, u32 button, int charge_frames = 0);


	//Position

	bool add_pos(int x, int y);
	bool set_pos(int x, int y);
	bool invalid_x(float x);
	bool invalid_y(float y);

	/*
		For classes with multiple instances in the FighterInstance table, it makes sense to be able to call their methods from the FighterInstance class instead
		of needing to manually iterate through all of them. It's technically bad practice to do these for ALL of these methods but it's more streamlined
		than, for example, having a "clear_hitbox_all" macro that clears the hitboxes from the FighterInstance, and having a "clear_hitbox" that can only
		be called from the Hitbox class
	*/

	//Hitbox
	
	void new_hitbox(int id, float damage, float chip_damage, float counterhit_damage_mul, int scale, GameCoordinate anchor, GameCoordinate offset, 
		int hitbox_kind, float meter_gain_on_hit, float meter_gain_on_counterhit, float meter_gain_on_block, int situation_hit, int hitlag, int hitstun, 
		int blocklag, int blockstun, bool unblockable, int attack_height, int attack_level, float hit_pushback, float block_pushback, int clank_kind, 
		bool success_hit, int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, float launch_init_y, 
		float launch_gravity_y, float launch_max_fall_speed, float launch_speed_x, bool use_player_pos = true);
	void update_hitbox_pos();
	void update_hitbox_connect();
	void clear_hitbox(int id);
	void clear_hitbox_all();
	
	//Grabbox
	
	void new_grabbox(int id, GameCoordinate anchor, GameCoordinate offset, int grabbox_kind, int situation_hit, u32 attacker_status_if_hit, 
		u32 defender_status_if_hit, bool use_player_pos = true);
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();
	
	//Hurtbox
	
	void new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind);
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	//Transitions

	bool is_actionable();
	bool can_kara();

	//Animation
	
	void change_anim(string animation_name, int frame_rate = 60, int entry_frame = 0);
	void startAnimation(Animation* animation);
	bool canStep();
	void stepAnimation();

	//Status

	bool change_status(u32 new_status_kind, bool call_end_status = true, bool require_different_status = true);
	void playoutStatus();
	bool common_ground_status_act();
	bool common_air_status_act();
	bool common_air_status_general();
	u32 get_status_group(u32 status_kind);

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
	virtual void status_grab_air();
	virtual void enter_status_grab_air();
	virtual void exit_status_grab_air();
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

	
	virtual void roy_status_fireball_start() {};
	virtual void roy_enter_status_fireball_start() {};
	virtual void roy_exit_status_fireball_start() {};
};