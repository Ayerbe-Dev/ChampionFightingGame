#pragma once
#include "Button.h"
#include "utils.h"
#include "Stage.h"
#include "MenuConstants.h"

class PlayerInfo {
public:
	int id;
	int chara_kind{ 0 };
	StageInfo stage_info;
	std::string name;
	Button button_info[BUTTON_MAX];
	SDL_GameController* controller{ NULL };
	unsigned int buffer_order[6]{ BUTTON_LP, BUTTON_MP, BUTTON_HP, BUTTON_LK, BUTTON_MK, BUTTON_HK };
	std::string crash_reason{"Crash Message Goes Here"};
	int stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
	int stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;

	PlayerInfo();
	PlayerInfo(int id);

	void poll_buttons(const Uint8* keyboard_state);
	void reset_buffer();
	void check_controllers();
	void set_default_button_mappings(int id);
	bool check_button_on(unsigned int button);
	bool check_button_input(unsigned int button);
	bool check_button_input(unsigned int button[], int length, int min_matches);
	bool check_button_trigger(unsigned int button);
	bool check_button_release(unsigned int button);
	bool is_valid_buffer_button(unsigned int button);
	void move_to_front(unsigned int buttons[6], unsigned int button);
	bool horizontal_input(bool right);
	bool vertical_input(bool down);
	bool is_any_inputs();
};