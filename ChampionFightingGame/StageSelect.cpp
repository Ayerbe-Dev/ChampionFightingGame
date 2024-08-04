#include "StageSelect.h"
#include "FontManager.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "GLM Helpers.h"
#include "WindowConstants.h"
#include "utils.h"

#include <fstream>

void stage_select_main() {
	FontManager* font_manager = FontManager::get_instance();
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	StageSelect* stage_select = new StageSelect;

	window_manager->update_shader_cams();
	window_manager->update_shader_lights();

	while (stage_select->looping) {
		game_manager->frame_delay_check_fps();

		window_manager->clear_screen();

		stage_select->process_game_state();
		stage_select->render_game_state();

		window_manager->update_screen();
	}

	StageDemo demo = stage_select->stages[stage_select->selection];
	while (demo.id == -1) { //Random
		demo = stage_select->stages[rng(0, stage_select->stages.size() - 1)];
	}

	switch (stage_select->game_context) {
		default: {
			for (int i = 0; i < 2; i++) {
				player[i]->stage_info = StageInfo(demo.id, demo.resource_name);
			}
		} break;
	}

	delete stage_select;
	ResourceManager::get_instance()->unload_unused();
}

StageDemo::StageDemo() {
	this->id = -1;
	this->name = "";
	this->resource_name = "";
}

StageDemo::StageDemo(int id, std::string name, std::string resource_name) {
	this->id = id;
	this->name = name;
	this->resource_name = resource_name;
	demo_model.load_model("resource/stage/" + resource_name + "/assets/main/model/model.dae");
	demo_model.init_shader();
	demo_anim.load_camera_anim("demo", "resource/stage/" + resource_name + "/cam_anims/demo.fbx");
	selected_anim.load_camera_anim("selected", "resource/stage/" + resource_name + "/cam_anims/selected.fbx");

	std::ifstream light_stream;
	light_stream.open("resource/stage/" + resource_name + "/assets/main/param/lights.yml");
	if (light_stream.fail()) {
		std::cout << "Failed to load lights!\n";
		light_stream.close();
		return;
	}

	glm::vec3 light_pos;
	glm::vec3 light_col;
	float brightness;
	while (light_stream >> light_pos.x) {
		light_stream >> light_pos.y >> light_pos.z >> light_col.x >> light_col.y >> light_col.z >> brightness;
		lights.emplace_back(light_pos, light_col, brightness);
	}
	light_stream.close();
}

StageDemo::StageDemo(StageDemo& other) {
	this->id = other.id;
	this->name = other.name;
	this->resource_name = other.resource_name;
	this->demo_model = other.demo_model;
	this->demo_anim = other.demo_anim;
	this->selected_anim = other.selected_anim;
	this->lights = other.lights;
}

StageDemo::StageDemo(const StageDemo& other) {
	this->id = other.id;
	this->name = other.name;
	this->resource_name = other.resource_name;
	this->demo_model = other.demo_model;
	this->demo_anim = other.demo_anim;
	this->selected_anim = other.selected_anim;
	this->lights = other.lights;
}

StageDemo& StageDemo::operator=(StageDemo& other) {
	if (this == &other) {
		return *this;
	}

	this->id = other.id;
	this->name = other.name;
	this->resource_name = other.resource_name;
	this->demo_model = other.demo_model;
	this->demo_anim = other.demo_anim;
	this->selected_anim = other.selected_anim;
	this->lights = other.lights;

	return *this;
}

StageDemo& StageDemo::operator=(const StageDemo& other) {
	if (this == &other) {
		return *this;
	}

	this->id = other.id;
	this->name = other.name;
	this->resource_name = other.resource_name;
	this->demo_model = other.demo_model;
	this->demo_anim = other.demo_anim;
	this->selected_anim = other.selected_anim;
	this->lights = other.lights;

	return *this;
}

StageSelect::StageSelect() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	selection = game_manager->player[0]->stage_info.stage_kind;
	num_slots_per_row = 1;
	selected = false;

	push_menu_object("Background", 3); {
		push_menu_texture("bg1", "resource/game_state/stage_select/bg_1.png");
		push_menu_texture("bg2", "resource/game_state/stage_select/bg_2.png");
		push_menu_texture("bg3", "resource/game_state/stage_select/bg_3.png");
	} pop_menu_stack();


	if (!load_stage_select()) {
		game_manager->add_crash_log("Could not load Stage Select!");
		return;
	}
	prev_selection = selection;

	window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
	stages[selection].demo_model.model.load_textures();
	for (size_t i = 0, max = stages[selection].lights.size(); i < max; i++) {
		window_manager->add_light(stages[selection].lights[i]);
	}
}

StageSelect::~StageSelect() {
	for (int i = 0, max = stages.size(); i < max; i++) {
		if (i != selection) {
			stages[i].demo_model.model.unload_textures();
			stages[i].demo_model.model.unload_model_instance();
		}
	}
	WindowManager* window_manager = WindowManager::get_instance();
	window_manager->remove_lights();
	window_manager->camera.reset_camera();
}

bool StageSelect::load_stage_select() {
	ParamTable stage_params("resource/game_state/stage_select/stage_select_param.prmlst");
	if (stage_params.load_failed()) {
		return false;
	}
	size_t list_start_offset = 1;
	FontManager* font_manager = FontManager::get_instance();
	Font main_text_font = font_manager->load_font("FiraCode", 12);
	stages.reserve(stage_params.get_param_int("num_slots"));
	push_menu_object("Stage Select"); {
		object_stack.top()->set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		push_menu_activity_group("Stage Slots", &selection, false, stage_params.get_param_int("num_slots")); {
			switch (game_context) { //Load the training mode versions of stages for training mode, else load the normal ones
				case (GAME_CONTEXT_TRAINING): {
					for (size_t i = list_start_offset, max = stages.capacity() + list_start_offset; i < max; i++) {
						if (stage_params.get_param_bool("selectable_training", i)) {
							add_stage_slot(stage_params.get_param_table(i), &main_text_font);
							if (selection == stages.back().id) { //This code is used to determine which slot index we hover over by default
								//given the player's selected stage.
								selection = stages.size() - 1;
							}
						}
					}
				} break;
				default: {
					for (size_t i = list_start_offset, max = stages.capacity() + list_start_offset; i < max; i++) {
						if (stage_params.get_param_bool("selectable_vs", i)) {
							add_stage_slot(stage_params.get_param_table(i), &main_text_font);
							if (selection == stages.back().id) {
								selection = stages.size() - 1;
							}
						}
					}
				}
			}

			if (selection >= stages.size()) {
				//If we managed to get to the stage select with a selected ID that doesn't exist in this context, this either means that:
				//A. We were selecting a variant of a stage that's only selectable in a different context, I.E. we went into vs. mode's 
				//stage select while we had training mode's Training Stage as our preferred stage
				//B. We reached the stage select with an ID that doesn't exist in ANY context, I.E. we were selecting a stage that
				//doesn't exist in the stage select param list. 

				//We'll probably handle A by merging IDs between variants but giving them different resource directories so that won't
				//be an issue when we make variants, but if we manage to trigger B then something has gone seriously wrong.
				GameManager::get_instance()->add_crash_log("Entered the Stage Select with a selected value of " + std::to_string(selection)
					+ ", which does not correspond to a valid stage ID in this Game Context!");
				return false;
			}

			MenuActivityGroup& stage_slots = *activity_group_stack.top();
			num_slots_per_row = ceil((float)stage_slots.num_children() / 2.0f);
			float slot_width = (WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row;
			float slot_height = WINDOW_HEIGHT / 9;

			for (int i = 0, max = stage_slots.num_children(); i < max; i++) {
				//Positioning the slots - Note that we should probably middle-orient these instead of 
				//setting them to bottom left so that the text can always be in the center of the slot. I'm
				//too lazy to calc the position right now since I'm focusing on getting the shrunken gbuffer
				//to render, but I'd imagine it's just a matter of changing how slot_pos is calculated.

				glm::vec3 slot_pos = glm::vec3(
					STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2 + (i * slot_width * 2) + slot_width,
					slot_height * 2 + 60,
					0.0
				);
				if (i >= num_slots_per_row) {
					if (max % 2) {
						slot_pos.x += slot_width / 2;
					}
					slot_pos.x -= (slot_width * 2) * num_slots_per_row;
					slot_pos.y -= slot_height * 2;
				}

				stage_slots.get_child(i).set_pos(slot_pos);

				stage_slots.get_child(i).get_texture(0).set_width(slot_width);
				stage_slots.get_child(i).get_texture(0).set_height(slot_height);
			}
		} pop_menu_stack();
		push_menu_child("Cursor"); {
			push_menu_texture("cursor", "resource/game_state/stage_select/cursor.png"); {
				float slot_width = (WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row;
				float slot_height = WINDOW_HEIGHT / 9;
				last_pushed_texture->set_width(slot_width);
				last_pushed_texture->set_height(slot_height);
			}
		} pop_menu_stack();
		object_stack.top()->get_activity_group("Stage Slots").get_child(selection).event_on_selected();
	} pop_menu_stack();

	main_text_font.unload_font();

	return true;
}

void StageSelect::add_stage_slot(ParamTable param_table, Font* font) {
	int stage_kind = param_table.get_param_int("stage_kind");
	std::string stage_name = param_table.get_param_string("stage_name");
	std::string resource_name = param_table.get_param_string("resource_name");
	stages.emplace_back(stage_kind, stage_name, resource_name);
	push_menu_child(stage_name); {
		push_menu_texture("slot_texture", "resource/stage/" + resource_name + "/slot_texture.png");
		push_menu_texture("name_text", *font, stage_name, glm::vec4(255.0, 255.0, 255.0, 255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		push_menu_on_selected_event_function([this](MenuObject* object) {
			object->get_parent().get_child("Cursor").set_pos(object->get_pos(), 6);
		});
	} pop_menu_stack();
}

void StageSelect::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (selection != prev_selection) {
			stages[prev_selection].demo_model.model.unload_textures();
			stages[selection].demo_model.model.load_textures();
			window_manager->camera.frame = 0.0;
			window_manager->remove_lights();
			for (size_t i = 0, max = stages[selection].lights.size(); i < max; i++) {
				window_manager->add_light(stages[selection].lights[i]);
			}
		}
		prev_selection = selection;
		if (window_manager->camera.get_anim_name() == "selected" && window_manager->camera.anim_end) {
			window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
		}
		window_manager->camera.follow_anim();
	}
	else {
		if (!window_manager->camera.anim_end) {
			window_manager->camera.follow_anim();
		}
	}
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
	}
}

void StageSelect::render_main() {
	WindowManager* window_manager = WindowManager::get_instance();
	window_manager->execute_buffered_events();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	window_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	stages[selection].demo_model.render_shadow();
	glCullFace(GL_BACK);

	window_manager->g_buffer.use();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_manager->shadow_map.bind_textures();

	stages[selection].demo_model.render();
	window_manager->render_ssao();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	render_menu_object("Background");
	glViewport(window_manager->res_width * 0.2, window_manager->res_height * 0.34, window_manager->res_width * 0.6, window_manager->res_height * 0.6);
	window_manager->g_buffer.render();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	
	glDepthMask(GL_FALSE);
	render_menu_object("Stage Select");
}

void StageSelect::event_up_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (selection >= num_slots_per_row) {
			selection -= num_slots_per_row;
			window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
		}
	}
	else {

	}
}

void StageSelect::event_down_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (selection + num_slots_per_row < stages.size()) {
			selection += num_slots_per_row;
			window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
		}
	}
	else {

	}
}

void StageSelect::event_left_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (selection != 0 && selection != num_slots_per_row) {
			selection--;
			window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
		}
	}
	else {

	}
}

void StageSelect::event_right_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (selection != num_slots_per_row - 1 && selection != get_menu_object("Stage Select").get_activity_group("Stage Slots").num_children() - 1) {
			selection++;
			window_manager->camera.play_camera_anim(&stages[selection].demo_anim, 1.0, 0.0);
		}
	}
	else {

	}
}

void StageSelect::event_select_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		WindowManager* window_manager = WindowManager::get_instance();
		window_manager->camera.frame = 0.0;
		window_manager->camera.play_camera_anim(&stages[selection].selected_anim, 1.0, 0.0);
		selected = true;
	}
	else {
		update_state(GAME_STATE_CHARA_SELECT);
	}
}

void StageSelect::event_back_press() {
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		update_state(GAME_STATE_MENU);
	}
	else {
		window_manager->camera.play_camera_anim(&stages[selection].selected_anim, -1.0, 0.0);
		selected = false;
	}
}