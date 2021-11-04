#pragma once
#include "utils.h"
#include "Button.h"

class PlayerInfo {
public:
	int id;
	int chara_kind{ 0 };
	string stage_kind{ "training_room_old"};
	string name;
	Buttons button_info[BUTTON_MAX];
	SDL_GameController* controller{ NULL };
	u32 buffer_order[6]{ BUTTON_LP, BUTTON_MP, BUTTON_HP, BUTTON_LK, BUTTON_MK, BUTTON_HK };
	string crash_reason{"Crash Message Goes Here"};
	int stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
	int stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;

	PlayerInfo();
	PlayerInfo(int id);

	void update_controller();
	void set_default_button_mappings(int id);
	void update_buttons(const Uint8* keyboard_state);
	bool check_button_on(u32 button);
	bool check_button_input(u32 button);
	bool check_button_input(u32 button[], int length, int min_matches);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
	bool is_valid_buffer_button(u32 button);
	void move_to_front(u32 buttons[6], u32 button);
	bool horizontal_input(bool right);
	bool vertical_input(bool down);
};