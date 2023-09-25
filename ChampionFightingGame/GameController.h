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

const int BUFFER_LP = 1;
const int BUFFER_MP = 2;
const int BUFFER_HP = 4;
const int BUFFER_LK = 8;
const int BUFFER_MK = 16;
const int BUFFER_HK = 32;
const int BUFFER_STICK_L = 64;
const int BUFFER_STICK_R = 128;
const int BUFFER_STICK_U = 256;
const int BUFFER_STICK_D = 512;

class GameController {
public:
	GameController();

	int check_controllers();
	void poll_buttons();

	void add_button_mapping(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerButton c_mapping);
	void add_button_mapping(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerAxis c_axis);
	void add_button_mapping(unsigned int button_kind);

	void set_button_mapping(unsigned int button_kind, unsigned int k_mapping);
	void set_button_mapping(unsigned int button_kind, SDL_GameControllerButton c_mapping);
	void set_button_mapping(unsigned int button_kind, SDL_GameControllerAxis c_axis);

	bool check_button_on(unsigned int button_kind);
	bool check_button_input(unsigned int button_kind);
	bool check_button_input(unsigned int button[], int length, int min_matches = 0);
	bool check_button_trigger(unsigned int button_kind);
	bool check_button_release(unsigned int button_kind);

	bool horizontal_input(bool right);
	bool vertical_input(bool down);
	bool is_any_inputs();

	void set_button_on(unsigned int button_kind, int duration = 1);
	void set_button_off(unsigned int button_kind);

	void add_buffer_button(unsigned int button_kind, unsigned int buffer_kind);
	bool is_valid_buffer_button(unsigned int button_kind);
	void reset_buffer();
	short get_buffer_code();

	void set_id(int id);
	void set_stick_hold_timer(int h, int v);
private:
	int id;
	int stick_hold_h_timer;
	int stick_hold_v_timer;

	std::unordered_map<unsigned int, unsigned int> button_map;
	std::unordered_map<unsigned int, unsigned int> key_map[2];
	std::unordered_map<SDL_GameControllerButton, unsigned int> controller_map[2];
	std::unordered_map<SDL_GameControllerAxis, unsigned int> axis_map[2];

	std::unordered_map<unsigned int, unsigned int> buffer_buttons;

	std::vector<Button> button_info;
	short buffer_code = 0;
	SDL_GameController* controller;
};