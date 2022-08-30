#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "StageSelectConstants.h"
#include "Light.h"

void stage_select_main();

struct StageDemo {
	StageDemo();
	StageDemo(int id, std::string name, std::string resource_dir, std::string default_music_Kind);
	int id;
	std::string name;
	std::string default_music_kind;
	GameObject demo_model;
	CameraAnim demo_anim;
	CameraAnim selected_anim;
	std::vector<Light> lights;
};

class StageSelect : public GameMenu {
public:
	StageSelect();

	void load_game_menu();
	bool load_stage_select();

	void process();
	void render();

	void event_up_press();
	void event_down_press();
	void event_left_press();
	void event_right_press();
	void event_select_press();
	void event_back_press();

	std::vector<StageDemo> stages;

	int num_slots_per_row;
	int selection;
	int prev_selection;
	bool selected;
};