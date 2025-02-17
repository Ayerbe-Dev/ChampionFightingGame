#include "StageSelect.h"
#include "GameManager.h"
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

		stage_select->process();
		stage_select->render();

		window_manager->update_screen();
	}

	StageDemo demo = *(StageDemo*)stage_select->get_active_element().ptr_var("stage_demo");
	while (demo.id == -1) { //Random
		demo = stage_select->stages[rng(0, stage_select->stages.size() - 1)];
	}

	switch (stage_select->context) {
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

	selected = false;

	root.add_element("Background", SceneElement({
		{"bg1", ScreenTexture("resource/scene/stage_select/bg_1.png")},
		{"bg2", ScreenTexture("resource/scene/stage_select/bg_2.png")},
		{"bg3", ScreenTexture("resource/scene/stage_select/bg_3.png")}
	}));

	if (!load_stage_select()) {
		game_manager->add_crash_log("Could not load Stage Select!");
		return;
	}

	StageDemo* demo = (StageDemo*)get_active_element().ptr_var("stage_demo");
	window_manager->camera.play_camera_anim(&demo->demo_anim, 1.0, 0.0);
	demo->demo_model.model.load_textures();
	for (size_t i = 0, max = demo->lights.size(); i < max; i++) {
		window_manager->add_light(demo->lights[i]);
	}
}

StageSelect::~StageSelect() {
	for (int i = 0, max = stages.size(); i < max; i++) {
		if (&get_active_element() != &root.get_child("Stage Slots").get_child(i)) {
			stages[i].demo_model.model.unload_textures();
			stages[i].demo_model.model.unload_model_instance();
		}
	}
	WindowManager* window_manager = WindowManager::get_instance();
	window_manager->remove_lights();
	window_manager->camera.reset_camera();
}

bool StageSelect::load_stage_select() {
	ParamTable stage_params("resource/scene/stage_select/stage_select_param.prmlst");
	if (stage_params.load_failed()) {
		return false;
	}
	int selection = GameManager::get_instance()->player[0]->stage_info.stage_kind;
	size_t list_start_offset = 1;
	load_font("main_text", "FiraCode", 48);
	
	int num_slots = 0;
	switch (context) {//Count the number of slots we're about to load
		case (SCENE_CONTEXT_TRAINING): {
			for (size_t i = list_start_offset, max = stage_params.get_param_int("num_slots") + list_start_offset; i < max; i++) {
				if (stage_params.get_param_bool("selectable_training", i)) {
					num_slots++;
				}
			}
		} break;
		default: {
			for (size_t i = list_start_offset, max = stage_params.get_param_int("num_slots") + list_start_offset; i < max; i++) {
				if (stage_params.get_param_bool("selectable_vs", i)) {
					num_slots++;
				}
			}
		}
	}
	
	stages.reserve(num_slots);

	int num_slots_per_row = ceil((float)num_slots / 2.0f);
	float slot_width = (WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row;
	float slot_height = WINDOW_HEIGHT / 9;
	load_event("stage_slot_activate", [this](SceneElement* element) {
		WindowManager* window_manager = WindowManager::get_instance();
		element->get_parent().get_screen_texture("Cursor").set_pos(element->get_pos(), 6);
		StageDemo* demo = (StageDemo*)element->ptr_var("stage_demo");
		window_manager->camera.play_camera_anim(&demo->demo_anim, 1.0, 0.0);
		demo->demo_model.model.load_textures();
		window_manager->remove_lights();
		for (size_t i = 0, max = demo->lights.size(); i < max; i++) {
			window_manager->add_light(demo->lights[i]);
		}
	});
	load_event("stage_slot_deactivate", [this](SceneElement* element) {
		((StageDemo*)element->ptr_var("stage_demo"))->demo_model.model.unload_textures();
	});
	load_event("stage_slot_up_press", [this, num_slots_per_row](SceneElement* element) {
		if (!selected) {
			set_active_element(&element->get_sibling(element->int_var("idx") - num_slots_per_row));
		}
	});
	load_event("stage_slot_down_press", [this, num_slots_per_row](SceneElement* element) {
		if (!selected) {
			std::cout << element->int_var("idx") << ", " <<  element->int_var("idx") + num_slots_per_row << "\n";
			std::cout << element->get_name() << ", " << element->get_sibling(element->int_var("idx") + num_slots_per_row).get_name() << "\n\n";
			set_active_element(&element->get_sibling(element->int_var("idx") + num_slots_per_row));

		}
	});
	load_event("stage_slot_left_press", [this](SceneElement* element) {
		if (!selected) {
			set_active_element(&element->get_prev_sibling());
		}
	});
	load_event("stage_slot_right_press", [this](SceneElement* element) {
		if (!selected) {
			set_active_element(&element->get_next_sibling());
		}
	});
	load_event("stage_slot_select_press", [this](SceneElement* element) {
		WindowManager* window_manager = WindowManager::get_instance();
		if (!selected) {
			WindowManager* window_manager = WindowManager::get_instance();
			window_manager->camera.play_camera_anim(&((StageDemo*)element->ptr_var("stage_demo"))->selected_anim, 1.0, 0.0);
			selected = true;
		}
		else {
			update_scene(SCENE_CHARA_SELECT, SCENE_CONTEXT_NONE);
		}
	});
	load_event("stage_slot_back_press", [this](SceneElement* element) {
		WindowManager* window_manager = WindowManager::get_instance();
		if (!selected) {
			update_scene(SCENE_MAIN_MENU, SCENE_CONTEXT_NONE);
		}
		else {
			window_manager->camera.play_camera_anim(&((StageDemo*)element->ptr_var("stage_demo"))->selected_anim, -1.0, 0.0);
			selected = false;
		}
	});

	SceneElement& stage_slots = root.add_element("Stage Slots", SceneElement().set_orientation(TEXTURE_BOTTOM_LEFT));
	switch (context) { //Load the training mode versions of stages for training mode, else load the normal ones
		case (SCENE_CONTEXT_TRAINING): {
			for (size_t i = list_start_offset, max = stages.capacity() + list_start_offset; i < max; i++) {
				if (stage_params.get_param_bool("selectable_training", i)) {
					add_stage_slot(stage_params.get_param_table(i), num_slots, num_slots_per_row, slot_width, slot_height);
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
					add_stage_slot(stage_params.get_param_table(i), num_slots, num_slots_per_row, slot_width, slot_height);
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

	root.get_child("Stage Slots").add_element("Cursor",
		ScreenTexture("resource/scene/stage_select/cursor.png")
		.set_base_width((WINDOW_WIDTH - STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2) / num_slots_per_row)
		.set_base_height(WINDOW_HEIGHT / 9)
	);

	set_active_element(&root.get_child("Stage Slots").get_child(selection));
	
	return true;
}

void StageSelect::add_stage_slot(ParamTable param_table, int num_slots, int num_slots_per_row, float slot_width, float slot_height) {
	int stage_kind = param_table.get_param_int("stage_kind");
	std::string stage_name = param_table.get_param_string("stage_name");
	std::string resource_name = param_table.get_param_string("resource_name");
	int index = stages.size();
	stages.emplace_back(stage_kind, stage_name, resource_name);
	WindowManager* window_manager = WindowManager::get_instance();

	glm::vec3 slot_pos = glm::vec3(
		STAGE_SLOT_UNITS_FROM_HORIZONTAL_EDGE * 2 + (index * slot_width * 2) + slot_width,
		slot_height * 2 + 60,
		0.0
	);
	if (index >= num_slots_per_row) {
		if (num_slots % 2) {
			slot_pos.x += slot_width / 2;
		}
		slot_pos.x -= (slot_width * 2) * num_slots_per_row;
		slot_pos.y -= slot_height * 2;
	}
	
	root.get_child("Stage Slots").add_element(stage_name, 
		SceneElement({
			{"slot_texture", ScreenTexture("resource/stage/" + resource_name + "/slot_texture.png")
				.set_base_width(slot_width).set_base_height(slot_height)},
			{"name_text", ScreenText(&get_font("main_text"), stage_name, TextSpecifier().border(2))},
		})
		.set_pos(slot_pos)
		.ptr_var("stage_demo", &stages[index])
		.int_var("idx", index)
		.add_event("activate", get_event("stage_slot_activate"))
		.add_event("deactivate", get_event("stage_slot_deactivate"))
		.add_event("select_press", get_event("stage_slot_select_press"))
		.add_event("back_press", get_event("stage_slot_back_press"))
	);
	if (index >= num_slots_per_row) {
		root.get_child("Stage Slots/" + stage_name).add_event("up_press", get_event("stage_slot_up_press"));
	}
	if (index + num_slots_per_row < num_slots) {
		root.get_child("Stage Slots/" + stage_name).add_event("down_press", get_event("stage_slot_down_press"));
	}
	if (index != 0 && index != num_slots_per_row) {
		root.get_child("Stage Slots/" + stage_name).add_event("left_press", get_event("stage_slot_left_press"));
	}
	if (index != num_slots_per_row - 1 && index != num_slots - 1) {
		root.get_child("Stage Slots/" + stage_name).add_event("right_press", get_event("stage_slot_right_press"));
	}
}

void StageSelect::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	if (!selected) {
		if (window_manager->camera.get_anim_name() == "selected" && window_manager->camera.anim_end) {
			window_manager->camera.play_camera_anim(&((StageDemo*)get_active_element().ptr_var("stage_demo"))->demo_anim, 1.0, 0.0);
		}
		window_manager->camera.follow_anim();
	}
	else {
		if (!window_manager->camera.anim_end) {
			window_manager->camera.follow_anim();
		}
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
	((StageDemo*)get_active_element().ptr_var("stage_demo"))->demo_model.render_shadow();
	glCullFace(GL_BACK);

	window_manager->g_buffer.use();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_manager->shadow_map.bind_textures();

	((StageDemo*)get_active_element().ptr_var("stage_demo"))->demo_model.render();
	window_manager->render_ssao();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	root.get_child("Background").render();
	glViewport(window_manager->res_width * 0.2, window_manager->res_height * 0.34, window_manager->res_width * 0.6, window_manager->res_height * 0.6);
	glEnable(GL_FRAMEBUFFER_SRGB);
	window_manager->g_buffer.render();
	glDisable(GL_FRAMEBUFFER_SRGB);
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	
	glDepthMask(GL_FALSE);
	root.get_child("Stage Slots").render();
}