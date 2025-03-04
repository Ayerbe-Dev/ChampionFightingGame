#pragma once
#include <map>
#include <set>
#include "GameController.h"
#include "ScreenText.h"
#include "ScreenTexture.h"

class InputManager {
public:
	InputManager(InputManager& other) = delete;
	void operator=(const InputManager& other) = delete;

	void check_controllers();
	void register_controller(int controller_id, GameController* controller);
	void unregister_controller(int controller_id);
	GameController* get_owner(int controller_id);

	std::map<int, bool> keyboard_state;
	std::set<int> available_controllers;

	bool is_using_text_input() const;
	void modify_text_input(unsigned int key, unsigned int mods);

	static InputManager* get_instance();
	void destroy_instance();
private:
	InputManager();
	std::map<int, GameController*> controller_map;
	ScreenText* text_input_text;
	std::string clipboard_string;

	static InputManager* instance;
};