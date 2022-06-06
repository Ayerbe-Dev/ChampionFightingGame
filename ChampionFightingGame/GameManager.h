#pragma once
#include "Player.h"
#include "GameMenu.h"
#include "GameState.h"
#include "MenuConstants.h"
#include <functional>
#include <queue>

class RenderManager;

class GameManager {
public:
	GameManager(GameManager& other) = delete;
	void operator=(const GameManager& other) = delete;

	RenderManager* render_manager;

	Player *player[2];
	int layer = 0;
	int* game_state;
	int* prev_game_state;
	int* game_context;
	int* prev_game_context;
	bool looping[MAX_LAYERS];

	void (*game_main[GAME_STATE_MAX])();
	void (*game_substate_main[GAME_SUBSTATE_MAX])();

	void set_game_state_functions(); //Assigns a function to each index of game_main

	void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
	void set_menu_info(GameMenu *menu_target, int init_hold_frames = 20, int hold_rate = 4);
	GameMenu *get_target(int layer = -1);

	//Handles stuff like if the user quits the program or goes full screen. You can also optionally
	//pass a function to execute based on the SDL Events (See DebugMenu for an example).
	void handle_window_events(std::function<void(SDL_Event*)> event_handler = nullptr);
	void handle_menus();

	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
	void event_start_press();
	void event_select_press();
	void event_back_press();
	void event_pause_press();
	void event_any_press();

	void add_crash_log(std::string crash_reason);
	bool get_crash_log(std::string* ret);
	bool is_crash();

	static GameManager* get_instance();
	void destroy_instance();
private:
	GameManager();
	static GameManager* instance;

	GameMenu* menu_target[MAX_LAYERS];

	bool is_up_press(int id);
	bool is_down_press(int id);
	bool is_left_press(int id);
	bool is_right_press(int id);

	bool is_any_menu_input(int id);

	int init_hold_frames = 20;
	int hold_rate = 4;

	int u_hold_frames[2];
	int d_hold_frames[2];
	int l_hold_frames[2];
	int r_hold_frames[2];
	std::queue<std::string> crash_log;
};
