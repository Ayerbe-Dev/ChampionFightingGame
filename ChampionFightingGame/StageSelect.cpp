#include "StageSelect.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Loader.h"
#include <fstream>
#include "GLM Helpers.h"

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
		stage_select->process();
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
	demo_model.init_shader();
	demo_model.scale *= glm::vec3(0.05); 
	demo_anim.load_camera_anim("demo", "resource/stage/" + resource_dir + "/cam_anims/demo.fbx");
	selected_anim.load_camera_anim("selected", "resource/stage/" + resource_dir + "/cam_anims/selected.fbx");

	std::ifstream light_stream;
	light_stream.open("resource/stage/" + resource_dir + "/assets/demo/param/lights.yml");
	if (light_stream.fail()) {
		std::cout << "Failed to load lights! \n";
		light_stream.close();
		return;
	}

	lights.reserve(MAX_LIGHT_SOURCES);
	glm::vec3 light_pos;
	while (light_stream >> light_pos.x) {
		light_stream >> light_pos.y;
		light_stream >> light_pos.z;
		lights.push_back(Light(light_pos));
	}
	light_stream.close();
}

StageSelect::StageSelect() {
	selection = 0;
	prev_selection = 0;
	num_slots_per_row = 1;
	selected = false;
}

void StageSelect::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	game_manager->set_menu_info(this);

	menu_objects.resize(STAGE_SELECT_GROUP_MAX);

	if (!load_stage_select()) {
		game_manager->add_crash_log("Could not open CSS file!");
		game_loader->finished = true;
		return;
	}

	float slot_width = (WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row;
	float slot_height = WINDOW_HEIGHT / 9;

	menu_objects[STAGE_SELECT_GROUP_MISC].push_back(MenuObject(this, nullptr, false));
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.init("resource/game_state/stage_select/cursor.png");
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.set_width(slot_width);
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.set_height(slot_height);
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);

	menu_objects[STAGE_SELECT_GROUP_MISC][0].add_texture("resource/game_state/stage_select/bg_1.png");
	menu_objects[STAGE_SELECT_GROUP_MISC][0].add_texture("resource/game_state/stage_select/bg_2.png");
	menu_objects[STAGE_SELECT_GROUP_MISC][0].add_texture("resource/game_state/stage_select/bg_3.png");

	for (int i = 0, max = menu_objects[STAGE_SELECT_GROUP_MISC][0].textures.size(); i < max; i++) {
		menu_objects[STAGE_SELECT_GROUP_MISC][0].textures[i].set_height_scale(1.5);
		menu_objects[STAGE_SELECT_GROUP_MISC][0].textures[i].set_width_scale(1.5);
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

	stage_file.close();

	num_slots_per_row = ceil(menu_objects[STAGE_SELECT_GROUP_SLOT].size() / 2);
	float slot_width = (WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row;
	float slot_height = WINDOW_HEIGHT / 9;
	for (int i = 0, max = menu_objects[STAGE_SELECT_GROUP_SLOT].size(); i < max; i++) { 
		//Positioning the slots
		glm::vec3 slot_pos = glm::vec3(
			STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2 + i * slot_width, 
			slot_height * 2 + 60, 
			0.0
		);
		if (i >= num_slots_per_row) {
			if (max % 2) {
				slot_pos.x += slot_width / 2;
			}
			slot_pos.x -= slot_width * num_slots_per_row;
			slot_pos.y -= slot_height * 2;
		}

		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[0].set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[1].set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);

		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[0].set_pos(slot_pos);
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[1].set_pos(slot_pos);

		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[0].set_width(slot_width);
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].textures[0].set_height(slot_height);
	}

	main_text_font.unload_font();

	return true;
}

void StageSelect::process() {
	RenderManager* render_manager = RenderManager::get_instance();
	if (!selected) {
		if (selection != prev_selection) {
			stages[prev_selection].demo_model.model.unload_textures();
			stages[selection].demo_model.model.load_textures();
			render_manager->camera.frame = 0.0;
			render_manager->remove_light();
			for (int i = 0, max = stages[selection].lights.size(); i < max; i++) {
				render_manager->add_light(&stages[selection].lights[i]);
			}
		}
		prev_selection = selection;
		render_manager->camera.follow_anim(&stages[selection].demo_anim);
		menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.set_target_pos(menu_objects[STAGE_SELECT_GROUP_SLOT][selection].textures[0].pos, 8);
	}
	render_manager->execute_buffered_events();
}

void StageSelect::render() {
	RenderManager* render_manager = RenderManager::get_instance();

	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	render_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	render_manager->g_buffer.use();
	glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_manager->shadow_map.bind_textures();

	stages[selection].demo_model.render();

	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	menu_objects[STAGE_SELECT_GROUP_MISC][0].render();
	glViewport(render_manager->s_window_width * 0.2, render_manager->s_window_height * 0.28, render_manager->s_window_width * 0.6, render_manager->s_window_height * 0.6);
	render_manager->g_buffer.render();
	glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);
	glDepthMask(GL_FALSE);

	for (int i = 0, max = menu_objects[STAGE_SELECT_GROUP_SLOT].size(); i < max; i++) {
		menu_objects[STAGE_SELECT_GROUP_SLOT][i].render();
	}
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.process();
	menu_objects[STAGE_SELECT_GROUP_MISC][0].cursor.render();
}

void StageSelect::event_up_press() {
	if (!selected) {
		if (selection >= num_slots_per_row) {
			selection -= num_slots_per_row;
		}
	}
}

void StageSelect::event_down_press() {
	if (!selected) {
		if (selection < num_slots_per_row) {
			selection += num_slots_per_row;
		}
	}
}

void StageSelect::event_left_press() {
	if (!selected) {
		if (selection != 0 && selection != num_slots_per_row) {
			selection--;
		}
	}
}

void StageSelect::event_right_press() {
	if (!selected) {
		if (selection != num_slots_per_row - 1 && selection != menu_objects[STAGE_SELECT_GROUP_SLOT].size() - 1) {
			selection++;
		}
	}
}

void StageSelect::event_select_press() {
	if (!selected) {

	}
}

void StageSelect::event_back_press() {
	if (!selected) {

	}
}