#pragma once
#include "ObjectInstance.h"
#include "ProjectileInstance.h"
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
#include "Game.fwd.h"
#include "Game.h"

class FighterInstance: public ObjectInstance {
public:
	//Haha interface go brrrrrrrrrrrr

	virtual void chara_id() = 0;

	PlayerInfo* player_info;
	int chara_kind;
	FighterInstanceAccessor* fighter_instance_accessor;
	ProjectileInstance* projectile_objects[MAX_PROJECTILES]{};
	IObject* projectile_instances[MAX_PROJECTILES]{};


	bool kara_enabled{ false };
	
	int prev_stick_dir;
	
	int chara_int[CHARA_INT_MAX]{ 0 };
	float chara_float[CHARA_FLOAT_MAX]{ 0.0 };
	bool chara_flag[CHARA_FLAG_MAX]{false};
	
	int connected_hitbox;
	int connected_grabbox;
	int connected_projectile_hitbox;
	bool crash_to_debug{ false };

	void (FighterInstance::* pStatus[CHARA_STATUS_MAX])();
	void (FighterInstance::* pEnter_status[CHARA_STATUS_MAX])();
	void (FighterInstance::* pExit_status[CHARA_STATUS_MAX])();

	//Constructors

	FighterInstance();
	FighterInstance(SDL_Renderer* renderer, PlayerInfo *player_info);

	void fighter_main();

	//Child Class Entry Points

	virtual void chara_main() {};

	virtual bool specific_ground_status_act() { return false; };
	virtual bool specific_air_status_act() { return false; };

	void create_jostle_rect(GameCoordinate anchor, GameCoordinate offset);

	//Projectiles
	void init_projectile(int id, GameCoordinate pos);
	void destroy_projectile(int id);

	//Setup

	void superInit(int id, SDL_Renderer* renderer);
	void load_anim_list(SDL_Renderer* renderer);
	void loadStatusFunctions();

	//Inputs

	void processInput();
	bool check_button_on(u32 button);
	bool check_button_input(u32 button);
	bool check_button_input(u32 buttons[], int length, int min_matches = 0);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	int get_stick_dir();
	int get_flick_dir();
	int get_special_input(int special_kind, u32 button, int charge_frames = 0);
	bool get_normal_cancel(int attack_kind, u32 button, int situation_kind);

	//Param Helper Funcs

	int get_param_int_special(string param);
	float get_param_float_special(string param);
	bool get_param_bool_special(string param);
	string get_param_string_special(string param);

	//Position

	bool add_pos(float x, float y, bool prev = false);
	bool set_pos(float x, float y, bool prev = false);

	//Messing with the opponent's FighterInstance

	void set_opponent_offset(GameCoordinate offset, int frames);
	void set_opponent_offset(GameCoordinate offset);
	void change_opponent_status(u32 status_kind);
	void damage_opponent(float damage, float facing_dir, float x_speed = 0, float y_speed = 0);
	void set_opponent_angle(double angle);
	void set_opponent_thrown_ticks();
	void change_opponent_anim(string anim_kind, int frame_rate = 1, int entry_frame = 0);

	/*
		For classes with multiple instances in the FighterInstance table, it makes sense to be able to call their methods from the FighterInstance class instead
		of needing to manually iterate through all of them. It's technically bad practice to do these for ALL of these methods but it's more streamlined
		than, for example, having a "clear_hitbox_all" macro that clears the hitboxes from the FighterInstance, and having a "clear_hitbox" that can only
		be called from the Hitbox class
	*/

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
	bool common_ground_status_act();
	bool common_air_status_act();
	bool common_air_status_general();
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

	virtual bool specific_status_attack() { return false; };
	
	virtual void roy_status_special_fireball_start() {};
	virtual void roy_enter_status_special_fireball_start() {};
	virtual void roy_exit_status_special_fireball_start() {};
	virtual void roy_status_special_fireball_punch() {};
	virtual void roy_enter_status_special_fireball_punch() {};
	virtual void roy_exit_status_special_fireball_punch() {};
	virtual void roy_status_special_fireball_kick() {};
	virtual void roy_enter_status_special_fireball_kick() {};
	virtual void roy_exit_status_special_fireball_kick() {};
	virtual void roy_status_special_uppercut_start() {};
	virtual void roy_enter_status_special_uppercut_start() {};
	virtual void roy_exit_status_special_uppercut_start() {};
	virtual void roy_status_special_uppercut() {};
	virtual void roy_enter_status_special_uppercut() {};
	virtual void roy_exit_status_special_uppercut() {};
	virtual void roy_status_special_uppercut_fall() {};
	virtual void roy_enter_status_special_uppercut_fall() {};
	virtual void roy_exit_status_special_uppercut_fall() {};

	virtual void eric_status_special_uppercut_start() {};
	virtual void eric_enter_status_special_uppercut_start() {};
	virtual void eric_exit_status_special_uppercut_start() {};
	virtual void eric_status_special_uppercut() {};
	virtual void eric_enter_status_special_uppercut() {};
	virtual void eric_exit_status_special_uppercut() {};
	virtual void eric_status_special_uppercut_fall() {};
	virtual void eric_enter_status_special_uppercut_fall() {};
	virtual void eric_exit_status_special_uppercut_fall() {};

	virtual void chara_template_status_template() {};
	virtual void chara_template_enter_status_template() {};
	virtual void chara_template_exit_status_template() {};
};