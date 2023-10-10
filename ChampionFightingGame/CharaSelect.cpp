#include "CharaSelect.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ThreadManager.h"
#include "Loader.h"
#include "GLM Helpers.h"
#include "PlayerInfo.h"
#include "utils.h"
#include "debug.h"
#include <fstream>

/// <summary>
/// The main function while on the character select screen.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void chara_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	CSS *css = new CSS;
	
	while (css->looping) {
		game_manager->frame_delay_check_fps();
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_manager->handle_window_events();

		css->process_game_state();
		css->render_game_state();

		SDL_GL_SwapWindow(render_manager->window);
	}

	delete css;
}

CSS::CSS() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	for (int i = 0; i < 2; i++) {
		css_player[i].init(player[i]->player_info);
	}

	gbuffer_window_counter = 20;
	gbuffer_mul = glm::vec4(0.2, 0.28, 0.6, 0.6);
	gbuffer_mul_offset = (glm::vec4(0.1, 0.32, 0.8, 0.78) - gbuffer_mul) / glm::vec4(gbuffer_window_counter);

	loaded_chars = 0;
	thread_loaded_chars = 0;

	num_cols = 0;
	num_rows = 0;
	cols_offset = 0;

	stage_demo.load_model(player[0]->stage_info.resource_dir + "assets/demo/model/model.dae");
	stage_demo.init_shader();
	stage_demo.model.load_textures();

	menu_objects.resize(CHARA_SELECT_GROUP_MAX);

	if (!load_css()) {
		game_manager->add_crash_log("Could not load CSS!");
		return;
	}

	ThreadManager* thread_manager = ThreadManager::get_instance();
	thread_manager->add_thread(THREAD_KIND_LOAD, gamestate_charaselect_loading_thread, this);
	thread_manager->notify_thread(THREAD_KIND_LOAD);

	menu_objects[CHARA_SELECT_GROUP_MISC].emplace_back(this, nullptr, false);
	menu_objects[CHARA_SELECT_GROUP_MISC].emplace_back(this, nullptr, true);

	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_1.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_2.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].add_texture("resource/game_state/chara_select/bg_3.png");

	for (int i = 0, max = menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures.size(); i < max; i++) {
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures[i].set_height_scale(1.5);
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].textures[i].set_width_scale(1.5);
	}

	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].add_texture("resource/game_state/chara_select/p1_cursor.png");
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].add_texture("resource/game_state/chara_select/p2_cursor.png");
	for (int i = 0; i < 2; i++) {
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].textures[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].textures[i].set_scale(0.8);
	}

	lights.reserve(MAX_LIGHT_SOURCES);
	std::ifstream light_stream;
	light_stream.open(player[0]->stage_info.resource_dir + "/assets/demo/param/lights.yml");
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
		render_manager->add_light(&lights.back());
	}
	light_stream.close();

	if (num_rows == 0) {
		css_player[0].my_col = 1;
		css_player[1].my_col = 1;
	}

	for (int i = 0; i < 2; i++) {
		player_id = i;
		if (player[i]->chara_kind != CHARA_KIND_MAX) {
			select_default_chara_kind(player[i]->chara_kind);
		}
		else {
			select_default_chara_kind(css_player[i].selected_index);
		}
		css_player[i].demo_model.rot.z = glm::radians(90.0f);
		if (!i) {
			css_player[i].demo_model.pos = glm::vec3(-350.0, 0.0, 0.0);
		}
		else {
			css_player[i].demo_model.pos = glm::vec3(350.0, 0.0, 0.0);
		}
	}
}

void gamestate_charaselect_loading_thread(void* charaselect_arg) {
	CSS* css = (CSS*)charaselect_arg;
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (size_t i = 0, max = css->chara_slots.size(); i < max; i++) {
		std::string resource_dir = "resource/chara/" + css->chara_slots[i].resource_dir;
		if (css->chara_slots[i].resource_dir != "default") {
			ModelData* model_data = ResourceManager::get_instance()->get_model_keep_user_count(resource_dir + "/model/m0/model.dae");
			css->chara_slots[i].anim_table.load_anlst(resource_dir + "/anims/demo", model_data->skeleton);
		}
		css->thread_loaded_chars++;
	}
}

CSS::~CSS() {
	RenderManager* render_manager = RenderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (size_t i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots[i].render_texture.destroy();
	}

	render_manager->remove_light();
	resource_manager->unload_unused();
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>0 if successful, -1 if the file fails to open.</returns>
bool CSS::load_css() {
	ParamTable css_params("resource/game_state/chara_select/css_param.prmlst");
	if (css_params.load_failed()) {
		return false;
	}
	size_t list_start_offset = 1; 
	//The first spot on this param list is the number of params, so entry 0 isn't a list. We COULD bake this offset into 
	//get_param_list(int), but I think handling it manually is ok since otherwise we'd need to store a separate vector of pointers
	for (size_t i = list_start_offset, max = css_params.get_param_int("num_slots") + list_start_offset; i < max; i++) {
		if (css_params.get_param_bool("selectable", i)) {
			chara_slots.push_back(CssSlot(css_params.get_param_table(i)));
		}
	}

	int col = 0;
	int row = 0;
	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots[i].my_col = col;
		chara_slots[i].my_row = row;
		if (col == 9) {
			col = 0;
			row++;
		}
		else {
			col++;
		}
	}
	num_cols = col;
	num_rows = row;

	cols_offset = NUM_COLS - num_cols;

	for (size_t i = 0, max = chara_slots.size(); i < max; i++) {
		if (chara_slots[i].my_row == num_rows && num_cols != NUM_COLS) {
			chara_slots[i].my_col += (cols_offset / 2);
		}
		chara_slots[i].render_texture.set_pos(glm::vec3(
			(float)chara_slots[i].my_col * chara_slots[i].render_texture.get_base_width() * -2.1 -200,
			(float)chara_slots[i].my_row * chara_slots[i].render_texture.get_base_height() * -1.7 - 200,
			0
		));
		chara_slots[i].render_texture.set_target_pos(glm::vec3(
			(float)chara_slots[i].my_col * chara_slots[i].render_texture.get_base_width() * 2.1 + 600,
			(float)chara_slots[i].my_row * chara_slots[i].render_texture.get_base_height() * 1.7 + 1490,
			0
		), chara_slots[i].my_col * 5 + chara_slots[i].my_row * 2 + 8);
	}

	for (size_t i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots_ordered[chara_slots[i].my_col][chara_slots[i].my_row] = &chara_slots[i];
	}

	return true;
}

void CSS::event_select_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			css_player[player_id].mobile_css_slot = GameTexture(chara_slots[css_player[player_id].selected_index].render_texture);

			if (player_id) {
				css_player[player_id].mobile_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_MIDDLE);
			}
			else {
				css_player[player_id].mobile_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
			}
			css_player[player_id].mobile_css_slot.set_pos(chara_slots[css_player[player_id].selected_index].render_texture.get_pos_vacuum(&css_player[player_id].mobile_css_slot));
			css_player[player_id].mobile_css_slot.set_target_pos(big_chara_slots[player_id].pos.get_val(), 16.0);
			if (css_player[player_id].selected_index < loaded_chars) {
				css_player[player_id].demo_model.change_anim("selected", 1.0f, 0.0f);
			}

			css_player[player_id].state = CHARA_SELECTION_STATE_SELECTED_CHARA;
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			css_player[player_id].state = CHARA_SELECTION_STATE_SELECTED_COLOR;
		} break;
		default: {

		} break;
	}
}
void CSS::event_back_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			update_state(GAME_STATE_STAGE_SELECT);
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			if (css_player[player_id].selected_index < loaded_chars) {
				css_player[player_id].demo_model.change_anim("deselected", 1.0f, 0.0f);
			}
			css_player[player_id].state = CHARA_SELECTION_STATE_DESELECTED;
		} break;
		default: {
			css_player[player_id].state = CHARA_SELECTION_STATE_SELECTED_CHARA;
		} break;
	}
}

void CSS::event_start_press() {
	if (css_player[0].state == CHARA_SELECTION_STATE_SELECTED_COLOR
		&& css_player[1].state == CHARA_SELECTION_STATE_SELECTED_COLOR) {
		for (int i = 0; i < 2; i++) {
			player[i]->chara_kind = chara_slots[css_player[i].selected_index].get_chara_kind();
			player[i]->alt_costume = css_player[i].selected_costume;
			player[i]->alt_color = css_player[i].selected_color;
		}
		update_state(GAME_STATE_BATTLE);
	}
}

void CSS::event_right_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			if (css_player[player_id].my_col != 9 && chara_slots_ordered[css_player[player_id].my_col + 1][css_player[player_id].my_row] != nullptr) {
				css_player[player_id].my_col++;
				css_player[player_id].selected_costume = 0;
				css_player[player_id].selected_color = 0;
				select_slot();
			}
			is_last_input_right[player_id] = true;
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			if (css_player[player_id].selected_color == chara_slots[css_player[player_id].selected_index].costumes[css_player[player_id].selected_costume].second - 1 ) {
				css_player[player_id].selected_color = -1;
			}
			css_player[player_id].selected_color++;
			select_color();
		} break;
		default: {

		} break;
	}
}

void CSS::event_left_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			if (css_player[player_id].my_col != 0 && chara_slots_ordered[css_player[player_id].my_col - 1][css_player[player_id].my_row] != nullptr) {
				css_player[player_id].my_col--;
				css_player[player_id].selected_costume = 0;
				css_player[player_id].selected_color = 0;
				select_slot();
			}
			is_last_input_right[player_id] = false;
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			if (css_player[player_id].selected_color == 0) {
				css_player[player_id].selected_color = chara_slots[css_player[player_id].selected_index].costumes[css_player[player_id].selected_costume].second;
			}
			css_player[player_id].selected_color--;
			select_color();
		} break;
		default: {

		} break;
	}
}

void CSS::event_down_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			bool jump = false;
			if (css_player[player_id].my_row < num_rows) {
				if (chara_slots_ordered[css_player[player_id].my_col][css_player[player_id].my_row + 1] == nullptr) {
					jump = true;
					bool valid_col = false;
					if (css_player[player_id].my_col >= 5) {
						for (int i = NUM_COLS - 1; i > 0; i--) {
							if (chara_slots_ordered[i][css_player[player_id].my_row + 1] != nullptr) {
								css_player[player_id].my_col = i;
								valid_col = true;
								break;
							}
						}
					}
					else {
						for (int i = 0; i < NUM_COLS; i++) {
							if (chara_slots_ordered[i][css_player[player_id].my_row + 1] != nullptr) {
								css_player[player_id].my_col = i;
								valid_col = true;
								break;
							}
						}
					}
					if (!valid_col) {
						GameManager::get_instance()->add_crash_log("Couldn't find a valid column!");
						update_state(GAME_STATE_DEBUG_MENU);
						return;
					}
				}
				css_player[player_id].my_row++;
				bool min = css_player[player_id].my_col <= cols_offset / 2;

				if (num_cols % 2) {
					if (!is_last_input_right[player_id] && css_player[player_id].my_row == num_rows && !min) {
						css_player[player_id].my_col--;
					}
					if (!jump) {
						is_last_input_right[player_id] = !is_last_input_right[player_id];
					}
				}
				css_player[player_id].selected_costume = 0;
				css_player[player_id].selected_color = 0;
				select_slot();
			}
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			if (css_player[player_id].selected_costume == chara_slots[css_player[player_id].selected_index].costumes.size() - 1) {
				css_player[player_id].selected_costume = -1;
			}
			css_player[player_id].selected_costume++;
			select_costume();
			if (css_player[player_id].selected_costume == player[player_id]->player_info->preferred_costume[chara_slots[css_player[player_id].selected_index].chara_kind]) {
				css_player[player_id].selected_color = player[player_id]->player_info->preferred_color[chara_slots[css_player[player_id].selected_index].chara_kind];
				select_color();
			}
			else {
				css_player[player_id].selected_color = 0;
			}
		} break;
		default: {

		} break;
	}
}

void CSS::event_up_press() {
	switch (css_player[player_id].state) {
		case (CHARA_SELECTION_STATE_DESELECTED): {
			bool jump = false;
			if (css_player[player_id].my_row != 0) {
				if (chara_slots_ordered[css_player[player_id].my_col][css_player[player_id].my_row - 1] == nullptr) {
					jump = true;
					bool valid_col = false;
					if (css_player[player_id].my_col >= 5) {
						for (int i = NUM_COLS - 1; i > 0; i--) {
							if (chara_slots_ordered[i][css_player[player_id].my_row - 1] != nullptr) {
								css_player[player_id].my_col = i;
								valid_col = true;
								break;
							}
						}
					}
					else {
						for (int i = 0; i < NUM_COLS; i++) {
							if (chara_slots_ordered[i][css_player[player_id].my_row - 1] != nullptr) {
								css_player[player_id].my_col = i;
								valid_col = true;
								break;
							}
						}
					}
					if (!valid_col) {
						GameManager::get_instance()->add_crash_log("Couldn't find a valid column!");
						update_state(GAME_STATE_DEBUG_MENU);
						return;
					}
				}
				css_player[player_id].my_row--;
				bool max = css_player[player_id].my_col >= NUM_COLS - 1;

				if (num_cols % 2) {
					if (is_last_input_right[player_id] && css_player[player_id].my_row == num_rows - 1 && !max) {
						css_player[player_id].my_col++;
					}
					if (!jump) {
						is_last_input_right[player_id] = !is_last_input_right[player_id];
					}
				}
				css_player[player_id].selected_costume = 0;
				css_player[player_id].selected_color = 0;
				select_slot();
			}
		} break;
		case (CHARA_SELECTION_STATE_SELECTED_CHARA): {
			if (css_player[player_id].selected_costume == 0) {
				css_player[player_id].selected_costume = chara_slots[css_player[player_id].selected_index].costumes.size();
			}
			css_player[player_id].selected_costume--;
			select_costume();
			if (css_player[player_id].selected_costume == player[player_id]->player_info->preferred_costume[chara_slots[css_player[player_id].selected_index].chara_kind]) {
				css_player[player_id].selected_color = player[player_id]->player_info->preferred_color[chara_slots[css_player[player_id].selected_index].chara_kind];
				select_color();
			}
			else {
				css_player[player_id].selected_color = 0;
			}
			
		} break;
		default: {

		} break;
	}
}

void CSS::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	for (int i = 0; i < 2; i++) {
		player[i]->controller.poll_buttons();
	}
	game_manager->process_game_state_events();

	ThreadManager* thread_manager = ThreadManager::get_instance();
	if (thread_manager->is_active(THREAD_KIND_LOAD)) {
		if (thread_loaded_chars > loaded_chars) {
			ResourceManager* resource_manager = ResourceManager::get_instance();
			resource_manager->init_gl_model("resource/chara/" + chara_slots[loaded_chars].resource_dir + "/model/m0/model.dae");

			for (int i = 0; i < 2; i++) {
				if (loaded_chars == css_player[i].selected_index && !css_player[i].demo_model.model.is_loaded()) {
					css_player[i].demo_model.load_model(
						"resource/chara/" + chara_slots[css_player[i].selected_index].resource_dir +
						"/model/m" + std::to_string(css_player[i].selected_costume) + "/model.dae",
						"c" + std::to_string(css_player[i].selected_color)
					);
					css_player[i].demo_model.init_shader();
					css_player[i].demo_model.anim_table = chara_slots[css_player[i].selected_index].anim_table;
					if (css_player[i].state == CHARA_SELECTION_STATE_DESELECTED) {
						css_player[i].demo_model.change_anim("deselected_wait", 1.0f, 0.0f);
					}
					else {
						css_player[i].demo_model.change_anim("selected_wait", 1.0f, 0.0f);
					}
				}
			}

			loaded_chars++;
		}
		else if (loaded_chars == chara_slots.size()) {
			thread_manager->kill_thread(THREAD_KIND_LOAD);
		}
	}
	if (gbuffer_window_counter != 0) {
		gbuffer_mul += gbuffer_mul_offset;
		gbuffer_window_counter--;
	}
}

/// <summary>
/// Render all CSS-related textures and models
/// </summary>
void CSS::render_main() {
	RenderManager* render_manager = RenderManager::get_instance();
	render_manager->execute_buffered_events();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	render_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	stage_demo.render_shadow();
	for (int i = 0; i < 2; i++) {
		if (css_player[i].selected_index < loaded_chars) {
			css_player[i].demo_model.animate(i);
			if (css_player[i].demo_model.is_anim_end) {
				if (css_player[i].demo_model.anim_kind->name == "selected") {
					css_player[i].demo_model.change_anim("selected_wait", 1.0f, 0.0f);
					css_player[i].demo_model.animate(i);
				}
				else if (css_player[i].demo_model.anim_kind->name == "deselected") {
					css_player[i].demo_model.change_anim("deselected_wait", 1.0f, 0.0f);
					css_player[i].demo_model.animate(i);
				}
			}
			css_player[i].demo_model.render_shadow(i);
		}
	}
	glCullFace(GL_BACK);

	render_manager->g_buffer.use();
	glViewport(0, 0, render_manager->res_width, render_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_manager->shadow_map.bind_textures();
	stage_demo.render();

	glDisable(GL_CULL_FACE);

	for (int i = 0; i < 2; i++) {
		if (css_player[i].selected_index < loaded_chars) {
			css_player[i].demo_model.render(i);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_BG].render();
	glViewport(render_manager->res_width * gbuffer_mul.x, render_manager->res_height * gbuffer_mul.y, render_manager->res_width * gbuffer_mul.z, render_manager->res_height * gbuffer_mul.w);
	render_manager->g_buffer.render();
	glViewport(0, 0, render_manager->res_width, render_manager->res_height);

	glDepthMask(GL_FALSE);

	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		chara_slots[i].render();
	}

	for (int i = 0; i < 2; i++) {
		big_chara_slots[i].render();

		if (css_player[i].state != CHARA_SELECTION_STATE_DESELECTED) {
			if (css_player[i].mobile_css_slot.pos != big_chara_slots[i].pos) {
				css_player[i].mobile_css_slot.add_rot(glm::vec3(0.0, 360.0 / 16.0, 0.0));
				css_player[i].mobile_css_slot.set_width_scale(css_player[i].mobile_css_slot.get_width_scale() + (0.7 / 16));
				css_player[i].mobile_css_slot.set_height_scale(css_player[i].mobile_css_slot.get_height_scale() + (0.7 / 16));
			}
			else {
				css_player[i].mobile_css_slot.set_rot(glm::vec3(0.0, 0.0, 0.0));
				css_player[i].mobile_css_slot.set_scale(1.5);
			}
			css_player[i].mobile_css_slot.process();
			css_player[i].mobile_css_slot.render();
		}
		menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].textures[i].set_target_pos(
			glm::vec3(chara_slots[css_player[i].selected_index].render_texture.pos.get_val().x, chara_slots[css_player[i].selected_index].render_texture.pos.get_val().y, 0.0f),
			8.0f
		);
	}
	menu_objects[CHARA_SELECT_GROUP_MISC][CHARA_SELECT_MISC_CURSOR].render();
}

/// <summary>
/// Returns the character bound to the CSS slot which a given player is selecting.
/// </summary>
/// <param name="player">: The player to get the character for.</param>
/// <returns></returns>
int CSS::get_chara_kind(int player) {
	return chara_slots[css_player[player].selected_index].get_chara_kind();
}

/// <summary>
/// Select the CSS slot that the cursor is hovering over. Works by comparing the column and row of each slot to the column and row of the player, so 
/// make sure to set that up properly if this function is copied into another menu.
/// </summary>
void CSS::select_slot() {
	for (int i = 0, max = chara_slots.size(); i < max; i++) {
		if (css_player[player_id].my_col == chara_slots[i].my_col
		&& css_player[player_id].my_row == chara_slots[i].my_row) {
			css_player[player_id].selected_index = i;
			break;
		}
	}
	css_player[player_id].selected_costume = player[player_id]->player_info->preferred_costume[chara_slots[css_player[player_id].selected_index].chara_kind];
	css_player[player_id].selected_color = player[player_id]->player_info->preferred_color[chara_slots[css_player[player_id].selected_index].chara_kind];

	big_chara_slots[player_id] = GameTexture(chara_slots[css_player[player_id].selected_index].render_texture);
	big_chara_slots[player_id].set_scale(1.5);


	if (player_id) {
		big_chara_slots[player_id].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_MIDDLE);
	}
	else {
		big_chara_slots[player_id].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	big_chara_slots[player_id].set_pos(glm::vec3(
		40 + (player_id * (WINDOW_WIDTH - 270)),
		70, 
		0));
	big_chara_slots[player_id].set_alpha((Uint8)127);
	
	if (css_player[player_id].selected_index < loaded_chars) {
		if (css_player[player_id].demo_model.model.is_loaded()) {
			css_player[player_id].demo_model.model.unload_model_instance();
		}
		css_player[player_id].demo_model.load_model(
			"resource/chara/" + chara_slots[css_player[player_id].selected_index].resource_dir + 
			"/model/m" + std::to_string(css_player[player_id].selected_costume) + "/model.dae", 
			"c" + std::to_string(css_player[player_id].selected_color)
		);
		css_player[player_id].demo_model.anim_table = chara_slots[css_player[player_id].selected_index].anim_table;
		if (css_player[player_id].state == CHARA_SELECTION_STATE_DESELECTED) {
			css_player[player_id].demo_model.change_anim("deselected_wait", 1.0f, 0.0f);
		}
		else {
			css_player[player_id].demo_model.change_anim("selected_wait", 1.0f, 0.0f);
		}
	}
}

void CSS::select_costume() {
	if (css_player[player_id].demo_model.model.is_loaded()) {
		css_player[player_id].demo_model.model.unload_model_instance();
	}
	css_player[player_id].demo_model.load_model(
		"resource/chara/" + chara_slots[css_player[player_id].selected_index].resource_dir +
		"/model/m" + std::to_string(css_player[player_id].selected_costume) + "/model.dae", 
		"c0"
	);
}

void CSS::select_color() {
	if (css_player[player_id].demo_model.model.is_loaded()) {
		css_player[player_id].demo_model.model.unload_textures();
		css_player[player_id].demo_model.model.load_textures("c" +
			std::to_string(css_player[player_id].selected_color)
		);
	}
}

/// <summary>
/// Takes the character that the current player is already selecting, and automatically puts their cursor on the first CSS slot corresponding to that
/// character. Note: This function is used for when you re-enter the CSS after having already selected a character, such as after a match.
/// </summary>
/// <param name="chara_kind">: The character whose CSS slot to select.</param>
void CSS::select_default_chara_kind(int chara_kind) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (chara_slots[i].get_chara_kind() == chara_kind) {
			css_player[player_id].my_col = chara_slots[i].my_col;
			css_player[player_id].my_row = chara_slots[i].my_row;
			select_slot();
			return;
		}
	}
}

CssPlayer::CssPlayer() {
	my_col = 0;
	my_row = 0;

	selected_index = 0;
	selected_color = 0;
	selected_costume = 0;
	state = CHARA_SELECTION_STATE_DESELECTED;
}

void CssPlayer::init(PlayerInfo* player_info) {
	selected_index = player_info->preferred_chara;
	selected_costume = player_info->preferred_costume[selected_index];
	selected_color = player_info->preferred_color[selected_index];
	state = CHARA_SELECTION_STATE_DESELECTED;
}

CssSlot::CssSlot() {
	chara_kind = CHARA_KIND_MAX;
	my_col = -1;
	my_row = -1;
	resource_dir = "";
	chara_name = "";
}

CssSlot::CssSlot(ParamTable param_table) {
	init(param_table);
}

/// <summary>
/// Initialize a CSS slot with the given values.
/// </summary>
/// <param name="chara_kind">: The chara_kind this slot will correspond to.</param>
/// <param name="resource_dir">: The directory where the CSS slot's render is located.</param>
/// <param name="name">: The name of the character for UI purposes.</param>
void CssSlot::init(ParamTable param_table) {
	//The param table passed contains the basic data for the character, their name, etc.

	chara_kind = param_table.get_param_int("chara_kind");
	resource_dir = param_table.get_param_string("resource_name");
	chara_name = param_table.get_param_string("chara_name");

	//It also contains a secondary table which lists each costume and how many alts it has
	//The costume name and its alt are given as tertiary tables on the secondary table

	ParamTable costume_table = param_table.get_param_table("costumes");
	for (int i = 0, max = param_table.get_param_int("num_costumes"); i < max; i++) {
		//We populate by asking the secondary table what the costume name and num_colors is for
		//each of its tertiary tables

		costumes.emplace_back(
			costume_table.get_param_string("costume_name", i), 
			costume_table.get_param_int("num_colors", i)
		);
	}

	render_texture.init("resource/game_state/chara_select/chara/" + resource_dir + "/render.png");
	render_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	render_texture.set_scale(0.8);
}

/// <summary>
/// Render the CSS Slot's texture.
/// </summary>
void CssSlot::render() {
	render_texture.process();
	render_texture.render();
}

/// <summary>
/// Get the character for a CSS slot. For the record, chara_kind is public so this function is kinda irrelevant.
/// </summary>
/// <returns>: The character for a CSS slot.</returns>
int CssSlot::get_chara_kind() const {
	return chara_kind;
}