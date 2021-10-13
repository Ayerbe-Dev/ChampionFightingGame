#pragma once
#include "utils.h"
#include <string>
#include <string>
#include "GameCoordinate.h"
#include "Button.h"
#include <SDL.h>
#include "Animation.h"
#include "ParamTable.h"
#include "Hitbox.h"

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
	Buttons button_info[BUTTON_MAX];
	int prev_stick_dir;
	int chara_int[CHARA_INT_MAX];
	float chara_float[CHARA_FLOAT_MAX];
	bool chara_bool[CHARA_BOOL_MAX];
	string resource_dir;
	SDL_Texture* current_texture;
	int frame;
	bool is_anim_end{ false };
	Animation* anim_kind;
	Animation animation_table[256];
	SDL_Rect frame_rect;
	StatsTable stats;
	u32 hold_ms;
	u32 last_frame_ms;
//	Hitbox hitboxes[10];
//	Hurtbox hurtboxes[10];
//	Grabbox grabboxes[10];

	void (PlayerInfo::* pStatus[CHARA_STATUS_MAX])();
	void (PlayerInfo::* pEnter_status[CHARA_STATUS_MAX])();
	void (PlayerInfo::* pExit_status[CHARA_STATUS_MAX])();

	//constructors
	PlayerInfo();
	PlayerInfo(int id, string chara_kind, SDL_Renderer* renderer);
	
	void startAnimation(Animation* animation);
	void change_anim(string animation_name, int frame_rate = 60, int entry_frame = 0);
	void load_anim_list(SDL_Renderer* renderer);
	void load_params();
	void stepAnimation();
	void superInit(SDL_Renderer* renderer);
	bool canStep();
	void loadDefaultButtonMap();
	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	i32 get_stick_dir();
	i32 get_flick_dir();
	bool is_actionable();
	bool change_status(u32 new_status_kind, bool call_end_status = true);
	bool common_ground_status_act();
	void processInput();
	void playoutStatus();
	void loadStatusFunctions();

	//great wall of status funcs
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
	void status_attack();
	void enter_status_attack();
	void exit_status_attack();
	void status_hitstun();
	void enter_status_hitstun();
	void exit_status_hitstun();
	void status_blockstun();
	void enter_status_blockstun();
	void exit_status_blockstun();
};