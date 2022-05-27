#pragma once
#include "Button.h"
#include <vector>
#include <unordered_map>

enum {
	GAME_CONTROLLER_UPDATE_REGISTERED,
	GAME_CONTROLLER_UPDATE_NONE,
	GAME_CONTROLLER_UPDATE_UNREGISTERED,
	GAME_CONTROLLER_UPDATE_MAX,
};

class GameController {
public:
	GameController();

	int check_controllers();
	void poll_buttons(const Uint8* keyboard_state);

	void add_button(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerButton c_mapping);
	void add_button(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerAxis c_axis);

	bool check_button_on(unsigned int button_kind);
	bool check_button_input(unsigned int button_kind);
	bool check_button_input(unsigned int button[], int length, int min_matches = 0);
	bool check_button_trigger(unsigned int button_kind);
	bool check_button_release(unsigned int button_kind);

	bool horizontal_input(bool right);
	bool vertical_input(bool down);
	bool is_any_inputs();

	bool is_valid_buffer_button(unsigned int button_kind);
	void reset_buffer();
	void sort_buffer(unsigned int button);

	int id;
	std::unordered_map<unsigned int, unsigned int> button_map;
	std::vector<Button> button_info;
	unsigned int buffer_order[6] = { BUTTON_LP, BUTTON_MP, BUTTON_HP, BUTTON_LK, BUTTON_MK, BUTTON_HK };
	SDL_GameController* controller;
	int stick_hold_h_timer;
	int stick_hold_v_timer;
};