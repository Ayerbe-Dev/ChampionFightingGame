#pragma once
#include "utils.h"
#include "Button.h"

class PlayerInfo {
public:
	int id;
	int chara_kind{ 0 };
	string name;
	Buttons button_info[BUTTON_MAX];
	SDL_GameController* controller{ NULL };

	PlayerInfo();
	PlayerInfo(int id);

	void set_default_buttons(int id);
	void update_buttons(const Uint8* keyboard_state);
	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
};