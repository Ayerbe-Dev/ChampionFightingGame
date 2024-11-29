#pragma once
#include "SceneElement.h"
#include "Mouse.h"

const int SCENE_NONE = 0;
const int SCENE_BATTLE = 1;
const int SCENE_MAIN_MENU = 2;
const int SCENE_CHARA_SELECT = 3;
const int SCENE_STAGE_SELECT = 4;
const int SCENE_CLOSE = 5;
const int SCENE_DEBUG_MENU = 6;
const int SCENE_TITLE_SCREEN = 7;
const int SCENE_CONTROLS = 8;
const int SCENE_PAUSE_BATTLE = 9;

const int SCENE_MAX = 10;

const int SCENE_CONTEXT_NONE = 0;
const int SCENE_CONTEXT_TRAINING = 1;
const int SCENE_CONTEXT_STORY = 2;
const int SCENE_CONTEXT_ARCADE = 3;
const int SCENE_CONTEXT_SPECIAL = 4;
const int SCENE_CONTEXT_ONLINE = 5;

const int SCENE_CONTEXT_MAX = 6;

class Player;
class Scene {
public:
	Scene();
	~Scene();

	void process();
	void render();

	virtual void process_pre_event();
	virtual void process_main();
	virtual void render_main();

	SceneElement& get_active_element();
	void set_active_element(SceneElement* element);
	void set_player_active_element(SceneElement* element);

	void update_scene(unsigned int scene, unsigned int context);
	void execute_event(std::string event_name);

	void load_font(std::string name, std::string font_name, int font_size);
	Font& get_font(std::string name);

	void load_event(std::string name, std::function<void(SceneElement*)> fn);
	std::function<void(SceneElement*)> get_event(std::string name);

	Player* player[2];
	Mouse mouse;

	int player_id;
	unsigned int context;
	bool looping;
	SceneElement root;

	std::map<std::string, Font> font_map;
	std::map<std::string, std::function<void (SceneElement*)>> event_map;
private:
	SceneElement* active_element[2];
};