#pragma once
#include "Player.h"
#include "PlayerInfo.h"
#include "Scene.h"
#include <functional>
#include <queue>
#include <stack>
#include <list>
#include <any>
#include <chrono>
#include "debug.h"
#include "ScreenText.h"

class GameManager {
public:
	GameManager(GameManager& other) = delete;
	void operator=(const GameManager& other) = delete;
	
	Player *player[2];
	int next_scene;
	int next_scene_context;

	void (*scene_main[SCENE_MAX])();

	void update_scene(int next_scene = SCENE_MAX, int next_scene_context = SCENE_CONTEXT_MAX);
	void set_scene(Scene *scene);
	Scene *get_scene(int depth = 0);
	void delete_scene();

	void process_events();
	void render();

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

	std::vector<Scene*> scenes;


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
