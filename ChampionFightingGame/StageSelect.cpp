#include "StageSelect.h"

void stage_select_main() {

}

StageDemo::StageDemo() {
	this->id = -1;
	this->name = "";
	this->default_music_kind = "";
}

StageDemo::StageDemo(int id, std::string name, std::string default_music_kind, std::string resource_dir) {
	this->id = id;
	this->name = name;
	this->default_music_kind = default_music_kind;
	demo_model.load_model(resource_dir + "/assets/demo/model/model.dae");
	demo_anim.load_camera_anim("demo", resource_dir + "/cam_anims/demo.fbx");
	selected_anim.load_camera_anim("selected", resource_dir + "/cam_anims/selected.fbx");
}

StageSelect::StageSelect() {

}

void StageSelect::load_game_menu() {

}