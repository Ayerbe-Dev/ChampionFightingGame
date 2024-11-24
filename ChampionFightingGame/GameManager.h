#pragma once
#include "Player.h"
#include "PlayerInfo.h"
#include "GameState.h"
#include <functional>
#include <queue>
#include <stack>
#include <list>
#include <any>
#include "debug.h"
#include "ScreenText.h"

class GameManager {
public:
	GameManager(GameManager& other) = delete;
	void operator=(const GameManager& other) = delete;
	
	Player *player[2];
	int next_game_state;
	int next_game_context;

	void (*game_main[GAME_STATE_MAX])();

	void update_state(int next_game_state = GAME_STATE_MAX, int next_game_context = GAME_CONTEXT_MAX);
	void set_game_state(GameState *game_state);
	GameState *get_game_state(int depth = 0);
	void delete_game_state();

	void process_events();
	void render_game_states();

	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
	void event_start_press();
	void event_select_press();
	void event_back_press();
	void event_page_left_press();
	void event_page_right_press();
	void event_frame_pause_press();
	void event_frame_advance_press();
	void event_record_input_press();
	void event_replay_input_press();
	void event_switch_input_press();
	void event_any_press();

	void add_crash_log(std::string crash_reason);
	bool get_crash_log(std::string* ret);
	bool is_crash();

	void frame_delay();
	void frame_delay_check_fps();

#ifdef DEBUG
	bool frame_capped;
	friend class DebugMenu;
#endif

	static GameManager* get_instance();
	void destroy_instance();
private:
	GameManager();
	static GameManager* instance;

	std::vector<GameState*> game_state;


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

	//FPS stuff

	Font fps_font;
	ScreenText fps_counter;
	int frame;
	int fps;
	int prev_fps;
	std::chrono::steady_clock::time_point last_second;
};
