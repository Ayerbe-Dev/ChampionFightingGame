#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "Button.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "RenderManager.h"
#include "GameManager.h"
#include "Battle.h"

class Fighter;
class GameRect;

class Debugger {
public:
	Debugger();

	Button button_info[BUTTON_DEBUG_MAX];
	int target{ 0 };
	bool print_frames{ false };
	bool zoom{ false };
	bool enabled{ true }; //Change this value to false to completely disable debug mode

	void poll_inputs(const Uint8* keyboard_state);

	bool check_button_on(unsigned int button);
	bool check_button_trigger(unsigned int button);

	void debug_mode(Fighter* target, GameRect* debug_rect, glm::vec2* debug_anchor, glm::vec2* debug_offset);
	void print_commands();
	void debug_query(std::string command, Fighter* target, Fighter* other);
};

void cotr_imgui_init();
void cotr_imgui_debug_dbmenu(GameManager* game_manager);
void cotr_imgui_debug_battle(Battle* battle);
void cotr_imgui_terminate();
