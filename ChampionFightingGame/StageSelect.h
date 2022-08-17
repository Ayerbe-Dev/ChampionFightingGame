#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Camera.h"

void stage_select_main();

struct StageDemo {
	StageDemo();
	StageDemo(int id, std::string name, std::string default_music_kind, std::string resource_dir);
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
};