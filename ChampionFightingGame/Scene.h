#pragma once
#include "SceneElement.h"
#include "Mouse.h"

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

	void update_scene(unsigned int scene, unsigned int flags);
	void execute_event(std::string event_name);

	void load_font(std::string name, std::string font_name, int font_size);
	Font& get_font(std::string name);

	Player* player[2];
	Mouse mouse;

	int player_id;
	unsigned int flags;
	bool looping;

	std::map<std::string, Font> font_map;
private:
	SceneElement* active_element;
protected:
	SceneElement root;
};