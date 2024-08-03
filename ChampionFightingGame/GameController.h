#pragma once
#include "Button.h"
#include <vector>
#include <unordered_map>

class GameController {
public:
	GameController();

	void reset_button_mappings();

	void check_controllers();
	void set_owns_keyboard(bool owns_keyboard);
	void poll_menu();
	void poll_fighter();
	void poll_from_input_code(unsigned short input_code);

	void add_button(unsigned int button_kind);
	void add_button_mapping(unsigned int button_kind, unsigned int k_mapping, unsigned int c_mapping);
	void add_button_axis(unsigned int button_kind, unsigned int k_mapping, unsigned int c_axis);

	void set_button_k_mapping(unsigned int button_kind, unsigned int k_mapping);
	void set_button_c_mapping(unsigned int button_kind, unsigned int c_mapping);
	void set_button_c_axis(unsigned int button_kind, unsigned int c_axis);

	bool check_button_on(unsigned int button_kind);
	bool check_button_input(unsigned int button_kind);
	bool check_button_input(unsigned int button[], int length, int min_matches = 0);
	bool check_button_trigger(unsigned int button_kind);
	bool check_button_release(unsigned int button_kind);

	bool horizontal_input(bool right);
	bool vertical_input(bool down);
	bool is_any_inputs();
	bool has_any_controller();

	void reset_all_buttons();
	void reset_buffer();
	unsigned short get_buffer_code();
	unsigned short get_buffer_lockout_code();
	unsigned short get_input_code();
	void set_buffer_lockout_code(short last_used_buffer_code);

	void set_stick_hold_timer(int h, int v);
	void set_hold_buffer(bool hold_buffer);

	void swap_player_controller(GameController* other);
	void reset_player_controller();

	void remove_controller();
private:
	int stick_hold_h_timer;
	int stick_hold_v_timer;

	std::unordered_map<unsigned int, unsigned int> key_map[2];
	std::unordered_map<unsigned int, unsigned int> button_map[2];
	std::unordered_map<unsigned int, unsigned int> axis_map[2];

	bool hold_buffer;
	std::vector<Button> button_info;
	unsigned short buffer_code = 0;
	unsigned short buffer_lockout_code = 0;
	unsigned short input_code = 0;
	
	int controller_id;
	int* player_controller_id;

	bool owns_keyboard;
	bool* player_owns_keyboard;
};