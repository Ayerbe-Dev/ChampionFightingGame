#pragma once
#include "Scene.h"
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

class StageSelect : public Scene {
public:
	StageSelect();
	~StageSelect();

	bool load_stage_select();
	void add_stage_slot(ParamTable param_table, int num_slots, int num_slots_per_row, float slot_width, float slot_height);

	void process_main();
	void render_main();

	std::vector<StageDemo> stages;

	bool selected;
};