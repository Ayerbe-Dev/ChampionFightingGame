#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Param.h"

#define STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE 200

class Font;

void stage_select_main();

struct StageDemo {
	StageDemo();
	StageDemo(int id, std::string name, std::string resource_name);
	StageDemo(StageDemo& other);
	StageDemo(const StageDemo& other);
	StageDemo& operator=(StageDemo& other);
	StageDemo& operator=(const StageDemo& other);
	
	int id;
	std::string name;
	std::string resource_name;
	GameObject demo_model;
	CameraAnim demo_anim;
	CameraAnim selected_anim;
	std::vector<Light> lights;
};

class StageSelect : public GameState {
public:
	StageSelect();
	~StageSelect();

	bool load_stage_select();
	void add_stage_slot(ParamTable param_table, Font* font);

	void process_main();
	void render_main();

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