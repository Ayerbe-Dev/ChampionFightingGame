#pragma once
#include "utils.h"
#include <string>
#include <string>
#include "GameCoordinate.h"
#include "Button.h"
#include <SDL.h>
#include "Animation.h"

class PlayerInfo{
public:
	i64 id;
	std::string chara_kind;
	GameCoordinate pos;
	GameCoordinate prevpos;
	f32 height;
	f32 width;
	bool facing_right{ true };
	f32 facing_dir{ 1.0 };
	u32 status_kind{ CHARA_STATUS_WAIT };
	u32 situation_kind{ CHARA_SITUATION_GROUND };
	void (*status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	void (*enter_status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	void (*exit_status_pointer[CHARA_STATUS_MAX])(PlayerInfo* player_info);
	Buttons button_info[BUTTON_MAX];
	std::string resource_dir;
	SDL_Texture* current_texture;
	int frame;
	bool is_anim_end{ false };
	Animation* anim_kind;
	SDL_Rect frame_rect;

	PlayerInfo();
	PlayerInfo(std::string chara_kind, SDL_Renderer* renderer);
	void startAnimation(Animation* animation);
	void stepAnimation();
	void loadDefaultButtonMap();
	void setStateLikePlayer1();
	void setStateLikePlayer2();
	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	i32 get_stick_dir();
	void change_status(u32 new_status_kind);
	void common_ground_status_act();
	void processInput();

	void wait();
	void enter_wait();
	void exit_wait();
	void walkf();
	void enter_walkf();
	void exit_walkf();
	void walkb();
	void enter_walkb();
	void exit_walkb();
	void dash();
	void enter_dash();
	void exit_dash();
	void dashb();
	void enter_dashb();
	void exit_dashb();
	void crouchd();
	void enter_crouchd();
	void exit_crouchd();
	void crouch();
	void enter_crouch();
	void exit_crouch();
	void crouchu();
	void enter_crouchu();
	void exit_crouchu();
	void jumpsquat();
	void enter_jumpsquat();
	void exit_jumpsquat();
	void jump();
	void enter_jump();
	void exit_jump();
	void attack();
	void enter_attack();
	void exit_attack();
	void hitstun();
	void enter_hitstun();
	void exit_hitstun();
	void blockstun();
	void enter_blockstun();
	void exit_blockstun();
};

//function<void(PlayerInfo*)> wait;
//function<void(PlayerInfo*)> enter_wait;
//function<void(PlayerInfo*)> exit_wait;
//function<void(PlayerInfo*)> walkf;
//function<void(PlayerInfo*)> enter_walkf;
//function<void(PlayerInfo*)> exit_walkf;
//function<void(PlayerInfo*)> walkb;
//function<void(PlayerInfo*)> enter_walkb;
//function<void(PlayerInfo*)> exit_walkb;
//function<void(PlayerInfo*)> dash;
//function<void(PlayerInfo*)> enter_dash;
//function<void(PlayerInfo*)> exit_dash;
//function<void(PlayerInfo*)> dashb;
//function<void(PlayerInfo*)> enter_dashb;
//function<void(PlayerInfo*)> exit_dashb;
//function<void(PlayerInfo*)> crouchd;
//function<void(PlayerInfo*)> enter_crouchd;
//function<void(PlayerInfo*)> exit_crouchd;
//function<void(PlayerInfo*)> crouch;
//function<void(PlayerInfo*)> enter_crouch;
//function<void(PlayerInfo*)> exit_crouch;
//function<void(PlayerInfo*)> crouchu;
//function<void(PlayerInfo*)> enter_crouchu;
//function<void(PlayerInfo*)> exit_crouchu;
//function<void(PlayerInfo*)> jumpsquat;
//function<void(PlayerInfo*)> enter_jumpsquat;
//function<void(PlayerInfo*)> exit_jumpsquat;
//function<void(PlayerInfo*)> jump;
//function<void(PlayerInfo*)> enter_jump;
//function<void(PlayerInfo*)> exit_jump;
//function<void(PlayerInfo*)> attack;
//function<void(PlayerInfo*)> enter_attack;
//function<void(PlayerInfo*)> exit_attack;
//function<void(PlayerInfo*)> hitstun;
//function<void(PlayerInfo*)> enter_hitstun;
//function<void(PlayerInfo*)> exit_hitstun;
//function<void(PlayerInfo*)> blockstun;
//function<void(PlayerInfo*)> enter_blockstun;
//function<void(PlayerInfo*)> exit_blockstun;
