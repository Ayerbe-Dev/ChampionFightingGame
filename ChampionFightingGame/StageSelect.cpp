#include "StageSelect.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Loader.h"
#include <fstream>

void stage_select_main() {
	FontManager* font_manager = FontManager::get_instance();
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	font_manager->load_face("FiraCode");

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];
	const Uint8* keyboard_state;

	StageSelect *stage_select = new StageSelect;
	stage_select->load_game_menu();

	while (*stage_select->looping) {
		wait_ms();
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();
		stage_select->render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	font_manager->unload_face("FiraCode");
	delete stage_select;
}

StageDemo::StageDemo() {
	this->id = -1;
	this->name = "";
	this->default_music_kind = "";
}

StageDemo::StageDemo(int id, std::string name, std::string resource_dir, std::string default_music_kind) {
	this->id = id;
	this->name = name;
	this->default_music_kind = default_music_kind;
	ResourceManager::get_instance()->load_model_instance("resource/stage/" + resource_dir + "/assets/demo/model/model.dae", &demo_model.model);
	demo_anim.load_camera_anim("demo", "resource/stage/" + resource_dir + "/cam_anims/demo.fbx");
	selected_anim.load_camera_anim("selected", "resource/stage/" + resource_dir + "/cam_anims/selected.fbx");
}

StageSelect::StageSelect() {

}

void StageSelect::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	menu_objects.resize(STAGE_SELECT_GROUP_MAX);

	if (!load_stage_select()) {
		game_manager->add_crash_log("Could not open CSS file!");
		game_loader->finished = true;
		return;
	}

}

bool StageSelect::load_stage_select() {
	std::ifstream stage_file;
	stage_file.open("resource/game_state/stage_select/stage_select_param.yml");
	if (stage_file.fail()) {
		stage_file.close();
		return false;
	}


	std::string stage_name;
	int id;
	std::string resource_dir;
	std::string default_music_kind;

	FontManager* font_manager = FontManager::get_instance();
	Font main_text_font = font_manager->load_font("FiraCode", 12);

	for (int i = 0; getline(stage_file, stage_name); i++) {
		stage_file >> id >> resource_dir >> default_music_kind;
		stages.push_back(StageDemo(id, stage_name, resource_dir, default_music_kind));
		menu_objects[STAGE_SELECT_GROUP_SLOT].push_back(MenuObject(this, nullptr, false));
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].add_texture("resource/stage/" + resource_dir + "/assets/demo/slot_texture.png");
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].add_texture(main_text_font, stage_name, glm::vec4(255.0, 255.0, 255.0, 255.0));
		getline(stage_file, stage_name);
	}

	main_text_font.unload_font();

	stage_file.close();
	return true;
}

void StageSelect::render() {

}