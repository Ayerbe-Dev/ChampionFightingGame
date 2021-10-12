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
	GameCoordinate pos;
	GameCoordinate prevpos;
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
	u32 hold_ms;
	u32 last_frame_ms;

	PlayerInfo();
	PlayerInfo(int id);
	void change_anim(string new_anim_kind, int entry_frame = 0, int div_rate = 60);
	void startAnimation(Animation* animation);
	void load_anim_list();
	void stepAnimation();
	void superInit();
	bool canStep();
	void loadDefaultButtonMap();
	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	i32 get_stick_dir();
	void change_status(u32 new_status_kind);
	void common_ground_status_act();
	void processInput();
};