#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "StageSelectConstants.h"

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
};

class StageSelect : public GameMenu {
public:
	StageSelect();

	std::vector<StageDemo> stages;
	std::vector<GameObject> charas;

	void load_game_menu();
	bool load_stage_select();

	void render();
};