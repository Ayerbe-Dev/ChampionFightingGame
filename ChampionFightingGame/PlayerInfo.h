#pragma once
#include "utils.h"
#include <string>
#include "GameCoordinate.h"
#include "Button.h"
#include <SDL.h>
#include "Animation.h"
#include "ParamTable.h"
#include "PlayerInfo.fwd.h"
#include "Box.fwd.h"
#include "Box.h"

class PlayerInfo{
public:
	i64 id;
	string chara_kind;

	GameCoordinate pos;
	GameCoordinate prevpos;
	bool facing_right{ true };
	f32 facing_dir{ 1.0 };

	u32 status_kind{ CHARA_STATUS_WAIT };
	u32 situation_kind{ CHARA_SITUATION_GROUND };
	
	Animation* anim_kind;
	Animation animation_table[256];
	int frame;
	int last_excute_frame;
	int excute_count;
	u32 hold_ms;
	u32 last_frame_ms;
	bool is_anim_end{ false };
	
	int prev_stick_dir;
	
	StatsTable stats;
	int chara_int[CHARA_INT_MAX];
	float chara_float[CHARA_FLOAT_MAX];
	bool chara_flag[CHARA_FLAG_MAX];
	
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];
	i32 connected_hitbox;
	
	Buttons button_info[BUTTON_MAX];
	string resource_dir;
	SDL_Texture* current_texture;
	SDL_Rect frame_rect;

	void (PlayerInfo::* pStatus[CHARA_STATUS_MAX])();
	void (PlayerInfo::* pEnter_status[CHARA_STATUS_MAX])();
	void (PlayerInfo::* pExit_status[CHARA_STATUS_MAX])();

	//Constructors

	PlayerInfo();
	PlayerInfo(int id, string chara_kind, SDL_Renderer* renderer);

	//Setup

	void superInit(SDL_Renderer* renderer);
	void load_anim_list(SDL_Renderer* renderer);
	void load_params();
	void loadDefaultButtonMap();
	void loadStatusFunctions();

	//Definitely not ACMD

	bool is_excute_frame(int excute_count, int frame);
	bool is_excute_wait(int excute_count, int frames);

	//Inputs

	void processInput();
	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	i32 get_stick_dir();
	i32 get_flick_dir();

	/*
		For classes with multiple instances in the PlayerInfo table, it makes sense to be able to call their methods from the PlayerInfo class instead
		of needing to manually iterate through all of them. It's technically bad practice to do these for ALL of these methods but it's more streamlined
		than, for example, having a "clear_hitbox_all" macro that clears the hitboxes from the PlayerInfo, and having a "clear_hitbox" that can only
		be called from the Hitbox class
	*/

	//Hitbox
	
	void new_hitbox(int id, f32 damage, f32 chip_damage, f32 counterhit_damage_mul, GameCoordinate anchor, GameCoordinate offset, int hitbox_kind,
		f32 meter_gain_on_hit, f32 meter_gain_on_counterhit, f32 meter_gain_on_block, int situation_hit, int hitlag, int hitstun, int blocklag,
		int blockstun, bool unblockable, int attack_height, int attack_level, f32 hit_pushback, f32 block_pushback, int clank_kind, bool success_hit,
		int juggle_set, int max_juggle, int hit_status, int counterhit_status, int counterhit_type, f32 launch_init_y, f32 launch_gravity_y,
		f32 launch_max_fall_speed, f32 launch_speed_x, bool use_player_pos = true);
	void update_hitbox_pos();
	void update_hitbox_connect();
	void clear_hitbox(int id);
	void clear_hitbox_all();
	
	//Grabbox
	
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();
	
	//Hurtbox
	
	void new_hurtbox(int id, GameCoordinate anchor, GameCoordinate offset, int hurtbox_kind, bool armor, int intangible_kind);
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	//Animation
	
	bool is_actionable();
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

	//don't worry, it'll get longer :)
	void status_wait();
	void enter_status_wait();
	void exit_status_wait();
	void status_walkf();
	void enter_status_walkf();
	void exit_status_walkf();
	void status_walkb();
	void enter_status_walkb();
	void exit_status_walkb();
	void status_dash();
	void enter_status_dash();
	void exit_status_dash();
	void status_dashb();
	void enter_status_dashb();
	void exit_status_dashb();
	void status_crouchd();
	void enter_status_crouchd();
	void exit_status_crouchd();
	void status_crouch();
	void enter_status_crouch();
	void exit_status_crouch();
	void status_crouchu();
	void enter_status_crouchu();
	void exit_status_crouchu();
	void status_jumpsquat();
	void enter_status_jumpsquat();
	void exit_status_jumpsquat();
	void status_jump();
	void enter_status_jump();
	void exit_status_jump();
	void status_fall();
	void enter_status_fall();
	void exit_status_fall();
	void status_attack();
	void enter_status_attack();
	void exit_status_attack();
	void status_hitstun();
	void enter_status_hitstun();
	void exit_status_hitstun();
	void status_hitstun_air();
	void enter_status_hitstun_air();
	void exit_status_hitstun_air();
	void status_blockstun();
	void enter_status_blockstun();
	void exit_status_blockstun();
	void status_parry_start();
	void enter_status_parry_start();
	void exit_status_parry_start();
	void status_parry();
	void enter_status_parry();
	void exit_status_parry();
	void status_crumple();
	void enter_status_crumple();
	void exit_status_crumple();
	void status_launch_start();
	void enter_status_launch_start();
	void exit_status_launch_start();
	void status_launch();
	void enter_status_launch();
	void exit_status_launch();
	void status_clank();
	void enter_status_clank();
	void exit_status_clank();
	void status_throw_tech();
	void enter_status_throw_tech();
	void exit_status_throw_tech();
	void status_landing();
	void enter_status_landing();
	void exit_status_landing();
	void status_landing_attack();
	void enter_status_landing_attack();
	void exit_status_landing_attack();
	void status_landing_hitstun();
	void enter_status_landing_hitstun();
	void exit_status_landing_hitstun();
};