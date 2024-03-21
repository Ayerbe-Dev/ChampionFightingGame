#include "CharaSelect.h"
#include "FontManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ThreadManager.h"
#include "GLM Helpers.h"
#include "PlayerInfo.h"
#include "debug.h"
#include <fstream>
#include "WindowConstants.h"

/// <summary>
/// The main function while on the character select screen.
/// </summary>
void chara_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	CSS *css = new CSS;
	
	while (css->looping) {
		game_manager->frame_delay_check_fps();
		render_manager->clear_screen();

		render_manager->handle_window_events();

		css->process_game_state();
		css->render_game_state();

		render_manager->update_screen();
	}

	delete css;
}

CSS::CSS() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();

	loaded_chars = 0;
	thread_loaded_chars = 0;

	stage_demo.load_model(player[0]->stage_info.resource_dir + "assets/demo/model/model.dae");
	stage_demo.init_shader();
	stage_demo.model.load_textures();

	menu_objects.reserve(4);

	push_menu_object("Background");
		push_menu_texture("bg", "resource/game_state/chara_select/background.png");
	pop_menu_stack();

	push_menu_object("Foreground", 8); {
		push_menu_texture("chara_deck", "resource/game_state/chara_select/chara_deck.png");
		last_pushed_texture->set_pos(glm::vec3(0.0f, -2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("top_table", "resource/game_state/chara_select/top_table.png");
		last_pushed_texture->set_pos(glm::vec3(0.0f, 2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("frame_l", "resource/game_state/chara_select/frame_l.png");
		last_pushed_texture->set_pos(glm::vec3(-2000.0f, 0.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("frame_r", "resource/game_state/chara_select/frame_r.png");
		last_pushed_texture->set_pos(glm::vec3(2000.0f, 0.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("panel_dl", "resource/game_state/chara_select/panel_dl.png");
		last_pushed_texture->set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("panel_dr", "resource/game_state/chara_select/panel_dr.png");
		last_pushed_texture->set_pos(glm::vec3(2000.0f, -2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("panel_ul", "resource/game_state/chara_select/panel_ul.png");
		last_pushed_texture->set_pos(glm::vec3(-2000.0f, 2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("panel_ur", "resource/game_state/chara_select/panel_ur.png");
		last_pushed_texture->set_pos(glm::vec3(2000.0f, 2000.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
	} pop_menu_stack();

	if (!load_css()) {
		game_manager->add_crash_log("Could not load CSS!");
		return;
	}

	ThreadManager* thread_manager = ThreadManager::get_instance();
	thread_manager->add_thread(THREAD_KIND_LOAD, gamestate_charaselect_loading_thread, this);
	thread_manager->notify_thread(THREAD_KIND_LOAD);

	push_menu_object("Player Cursors"); {
		for (int i = 0; i < 2; i++) {
			push_menu_child("P" + std::to_string(i+1) + " Cursor"); {
				push_menu_int_var("selected_slot", 0);
				push_menu_int_var("selected_costume", 0);
				push_menu_int_var("selected_color", 0);
				push_menu_int_var("prev_selected_slot", 0);
				push_menu_int_var("chara_selection_state", CHARA_SELECTION_STATE_CHARA_HOVER);
				push_menu_bool_var("last_input_right", true);

				push_menu_activity_group("Selection State AG", &object_stack.top()->int_var("chara_selection_state"), true, 7); {
					push_menu_child("Selection State Name Entry Hover"); {
						//Put the css cursor over the name entry button
					} pop_menu_stack();
					push_menu_child("Selection State Name Entry"); {
						//Actually handle the name entry drop down
					} pop_menu_stack();
					push_menu_child("Selection State Controls Hover"); {
						//Put the css cursor over the controls button
					} pop_menu_stack();
					push_menu_child("Selection State Controls"); {
						//Controls dropdown
					} pop_menu_stack();
					push_menu_child("Selection State Chara Hover"); {
						push_menu_on_selected_event_function([this](MenuObject* object) {
							object->get_parent().get_texture("Chara Slot Large").set_alpha(127);
						});
					} pop_menu_stack();
					push_menu_child("Selection State Chara"); {
						push_menu_texture("Chara Slot Mobile");
						push_menu_process_function([this](MenuObject* object) {
							GameTexture& mobile_css_slot = object->get_texture("Chara Slot Mobile");
							GameTexture& large_css_slot = object->get_parent().get_texture("Chara Slot Large");
							if (mobile_css_slot.pos != large_css_slot.pos) {
								mobile_css_slot.add_rot(glm::vec3(0.0, 360.0 / 16.0, 0.0));
								mobile_css_slot.set_width_scale(mobile_css_slot.get_width_scale() + (0.7 / 16));
								mobile_css_slot.set_height_scale(mobile_css_slot.get_height_scale() + (0.7 / 16));
							}
							else {
								mobile_css_slot.set_alpha(0);
								large_css_slot.set_alpha(255);
							}
						});
						push_menu_on_selected_event_function([this, i](MenuObject* object) {
							if (object->get_parent().get_activity_group("Selection State AG").prev_active_index
								== CHARA_SELECTION_STATE_READY) return;
							GameTexture& mobile_css_slot = object->get_texture("Chara Slot Mobile");
							GameTexture& chara_render = get_menu_object("Chara Slots").get_child(
								object->get_parent().int_var("selected_slot")
							).get_texture("Chara Render");
							mobile_css_slot = GameTexture(chara_render);
							if (i) {
								mobile_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
							}
							else {
								mobile_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
							}
							mobile_css_slot.set_pos(chara_render.get_pos_vacuum(&mobile_css_slot));
							mobile_css_slot.set_target_pos(object->get_parent().get_texture("Chara Slot Large").pos, 16);
						});
						push_menu_on_deselected_event_function([&](MenuObject* object) {
							object->get_texture("Chara Slot Mobile").set_alpha(0);
						});
					} pop_menu_stack();
					push_menu_child("Selection State Ready"); {
						//"Press Start" texture
					} pop_menu_stack();
				} pop_menu_stack();

				GameTexture& default_chara = get_menu_object("Chara Slots").get_child(0).get_texture("Chara Render");

				push_menu_texture("Chara Slot Large");
				*last_pushed_texture = GameTexture(default_chara);
				last_pushed_texture->set_scale(1.25f);
				if (i) {
					last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
				}
				else {
					last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
				}
				last_pushed_texture->set_pos(glm::vec3(130.5, 338, 0));
				last_pushed_texture->set_alpha(127);
				push_menu_texture("Cursor", "resource/game_state/chara_select/p" 
					+ std::to_string(i+1) + "_cursor.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
				last_pushed_texture->set_target_pos(default_chara.pos.get_target_val(), 16);

				push_menu_up_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER): {
							std::string neighbor_val = "up_left_neighbor";
							if (object->bool_var("last_input_right")) {
								neighbor_val = "up_right_neighbor";
							}
							object->int_var("prev_selected_slot") = object->int_var("selected_slot");
							object->int_var("selected_slot") = get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var(neighbor_val);
							if (object->int_var("selected_slot") == -1) {
								object->int_var("chara_selection_state") = CHARA_SELECTION_STATE_CONTROLS_HOVER;
								return;
							}
							object->bool_var("last_input_right") = !object->bool_var("last_input_right");
							select_slot(i);
						} break;
						case (CHARA_SELECTION_STATE_CHARA): {
							if (object->int_var("selected_costume") == 0) {
								object->int_var("selected_costume") = get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var("num_costumes") - 1;
							}
							else {
								object->int_var("selected_costume")--;
							}
							select_costume(i);
						} break;
						case (CHARA_SELECTION_STATE_CONTROLS): {
							
						} break;
						case (CHARA_SELECTION_STATE_NAME_ENTRY_HOVER): {
							object->int_var("selected_slot") = object->int_var("prev_selected_slot");
							object->int_var("chara_selection_state") = CHARA_SELECTION_STATE_CHARA_HOVER;
						} break;
						case (CHARA_SELECTION_STATE_NAME_ENTRY): {

						} break;
					}
				});
				push_menu_down_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER): {
							std::string neighbor_val = "down_left_neighbor";
							if (object->bool_var("last_input_right")) {
								neighbor_val = "down_right_neighbor";
							}
							object->int_var("prev_selected_slot") = object->int_var("selected_slot");
							object->int_var("selected_slot") = get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var(neighbor_val);
							if (object->int_var("selected_slot") == -1) {
								object->int_var("chara_selection_state") = CHARA_SELECTION_STATE_NAME_ENTRY_HOVER;
								return;
							}
							object->bool_var("last_input_right") = !object->bool_var("last_input_right");
							select_slot(i);
						} break;
						case (CHARA_SELECTION_STATE_CHARA): {
							if (object->int_var("selected_costume") + 1 == get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var("num_costumes")) {
								object->int_var("selected_costume") = 0;
							}
							else {
								object->int_var("selected_costume")++;
							}
							select_costume(i);
						} break;
						case (CHARA_SELECTION_STATE_CONTROLS_HOVER): {
							object->int_var("selected_slot") = object->int_var("prev_selected_slot");
							object->int_var("chara_selection_state") = CHARA_SELECTION_STATE_CHARA_HOVER;
						} break;
						case (CHARA_SELECTION_STATE_CONTROLS): {

						} break;
						case (CHARA_SELECTION_STATE_NAME_ENTRY): {

						} break;
					}
				});
				push_menu_left_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER): {
							int new_selection = get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var("left_neighbor");
							if (new_selection == -1) return;
							object->int_var("prev_selected_slot") = object->int_var("selected_slot");
							object->int_var("selected_slot") = new_selection;
							object->bool_var("last_input_right") = false;
							select_slot(i);
						} break;
						case (CHARA_SELECTION_STATE_CHARA): {
							MenuObject& chara_slot = get_menu_object("Chara Slots").get_child(object->int_var("selected_slot"));
							if (object->int_var("selected_color") == 0) {
								object->int_var("selected_color") = chara_slot.int_var("costume_" + 
								std::to_string(object->int_var("selected_costume")) + "_num_colors") - 1;
							}
							else {
								object->int_var("selected_color")--;
							}
							select_color(i);
						} break;
						case (CHARA_SELECTION_STATE_NAME_ENTRY): {

						} break;
					}
				});
				push_menu_right_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER): {
							int new_selection = get_menu_object("Chara Slots")
								.get_child(object->int_var("selected_slot")).int_var("right_neighbor");
							if (new_selection == -1) return;
							object->int_var("prev_selected_slot") = object->int_var("selected_slot");
							object->int_var("selected_slot") = new_selection;
							object->bool_var("last_input_right") = true;
							select_slot(i);
						} break;
						case (CHARA_SELECTION_STATE_CHARA): {
							MenuObject& chara_slot = get_menu_object("Chara Slots").get_child(object->int_var("selected_slot"));
							if (object->int_var("selected_color") + 1 == chara_slot
								.int_var("costume_" + std::to_string(object->int_var("selected_costume"))
									+ "_num_colors")) {
								object->int_var("selected_color") = 0;
							}
							else {
								object->int_var("selected_color")++;
							}
							select_color(i);
						} break;
						case (CHARA_SELECTION_STATE_NAME_ENTRY): {

						} break;
					}
				});
				push_menu_select_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER): {
							demo_models[i].change_anim("selected", 1.0f, 0.0f);
						} [[fallthrough]];
						case (CHARA_SELECTION_STATE_NAME_ENTRY_HOVER):
						case (CHARA_SELECTION_STATE_CONTROLS_HOVER):
						case (CHARA_SELECTION_STATE_CHARA): {
							object->int_var("chara_selection_state")++;
						} break;
					}
				});
				push_menu_start_event_function([this](MenuObject* object) {
					if (object->int_var("chara_selection_state") == CHARA_SELECTION_STATE_CONTROLS) {
						object->int_var("chara_selection_state")--;
					}
				});
				push_menu_back_event_function([this, i](MenuObject* object) {
					switch (object->int_var("chara_selection_state")) {
						case (CHARA_SELECTION_STATE_CHARA_HOVER):
						case (CHARA_SELECTION_STATE_CONTROLS_HOVER):
						case (CHARA_SELECTION_STATE_NAME_ENTRY_HOVER): {
							ThreadManager* thread_manager = ThreadManager::get_instance();
							if (thread_manager->is_active(THREAD_KIND_LOAD)) {
								thread_manager->kill_thread(THREAD_KIND_LOAD);
							}
							update_state(GAME_STATE_STAGE_SELECT);
						} break;
						case (CHARA_SELECTION_STATE_CHARA): {
							demo_models[i].change_anim("deselected", 1.0f, 0.0f);
						} [[fallthrough]];
						case (CHARA_SELECTION_STATE_NAME_ENTRY):
						case (CHARA_SELECTION_STATE_READY): {
							object->int_var("chara_selection_state")--;
						} break;
					}
				});
			} pop_menu_stack();
		}
		push_menu_texture("slot_hover_l", "resource/game_state/chara_select/slot_hover_l.png");
		last_pushed_texture->set_pos(glm::vec3(-2000.0f, 0.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
		push_menu_texture("slot_hover_r", "resource/game_state/chara_select/slot_hover_r.png");
		last_pushed_texture->set_pos(glm::vec3(2000.0f, 0.0f, 0.0f));
		last_pushed_texture->set_target_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16);
	} pop_menu_stack();

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

	for (int i = 0; i < 2; i++) {
		player_id = i;
		if (player[i]->chara_kind != CHARA_KIND_MAX) {
			select_default_chara_kind(player[i]->chara_kind);
		}
		demo_models[i].rot.z = glm::radians(90.0f);
		if (!i) {
			demo_models[i].pos = glm::vec3(-350.0, 0.0, 0.0);
		}
		else {
			demo_models[i].pos = glm::vec3(350.0, 0.0, 0.0);
		}
	}
}

void gamestate_charaselect_loading_thread(void* charaselect_arg) {
	CSS* css = (CSS*)charaselect_arg;
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = css->get_menu_object("Chara Slots").num_children(); i < max; i++) {
		MenuObject& chara_slot = css->get_menu_object("Chara Slots").get_child(i);
		std::string resource_dir = "resource/chara/" + chara_slot.string_var("resource_name");
		ModelData* model_data = ResourceManager::get_instance()->get_model_keep_user_count(resource_dir + "/model/m0/model.dae");
		css->demo_anim_tables.push_back(AnimationTable());
		if (model_data->is_loaded()) {
			css->demo_anim_tables.back().load_anlst(resource_dir + "/anims/demo", model_data->skeleton);
		}
		css->thread_loaded_chars++;
	}
}

CSS::~CSS() {
	RenderManager* render_manager = RenderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	render_manager->remove_light();
	resource_manager->unload_unused();
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>true if successful, false if the file fails to open.</returns>
bool CSS::load_css() {
	ParamTable css_params("resource/game_state/chara_select/css_param.prmlst");
	if (css_params.load_failed()) {
		return false;
	}
	size_t css_param_sub_table_start = 2;
	/*
		Passing an offset n to ParamTable::get_param_table() will give you the nth param in the 
		ParamTable that calls it, NOT the nth param which is of type ParamTable. This is fine because
		iterating through nested param tables usually implies that those param tables are back to back,
		but since the first few params in css_param.prmlst are metadata, not param tables, we need
		to keep list_start_offset in order to make sure our nested param access is correct.
	*/
	int num_slots = css_params.get_param_int("num_slots");
	int num_cols = css_params.get_param_int("num_cols");
	int num_rows = num_slots / num_cols + 1;
	int num_cols_bottom_row = num_slots % num_cols;
	int l_offset = (num_cols - num_cols_bottom_row) / 2;
	int r_offset = l_offset + num_cols_bottom_row;
	bool diagonal_neighbors = (num_cols % 2) != (num_cols_bottom_row % 2);

	demo_anim_tables.reserve(num_slots);
	push_menu_object("Chara Slots", 0, num_slots); {
		push_menu_int_var("num_slots", num_slots);
		for (int i = 0; i < num_slots; i++) {
			ParamTable param_table = css_params.get_param_table(i + css_param_sub_table_start);
			std::string resource_name = param_table.get_param_string("resource_name");
			int num_costumes = param_table.get_param_int("num_costumes");
			ParamTable costume_table = param_table.get_param_table("costumes");
			float my_col = i % num_cols;
			int my_row = i / num_cols;
			if (my_row == num_rows - 1) {
				my_col += l_offset;
				if (diagonal_neighbors) {
					my_col += 0.5f;
				}
			}

			push_menu_child("chara_slot_" + resource_name); {
				push_menu_int_var("chara_kind", param_table.get_param_int("chara_kind"));
				push_menu_string_var("resource_name", resource_name);
				push_menu_int_var("num_costumes", num_costumes);
				for (int i = 0; i < num_costumes; i++) {
					push_menu_string_var(
						"costume_" + std::to_string(i) + "_name",
						costume_table.get_param_string("costume_name", i)
					);
					push_menu_int_var(
						"costume_" + std::to_string(i) + "_num_colors",
						costume_table.get_param_int("num_colors", i)
					);
				}
				push_menu_bool_var("selectable", 
					param_table.get_param_bool("selectable")
				//	|| TODO: Put a check for if a character has been unlocked, probably using SaveManager
				);
				if (my_row == 0) {
					push_menu_int_var("up_left_neighbor", -1);
					push_menu_int_var("up_right_neighbor", -1);
				}
				else {
					if (diagonal_neighbors) {
						push_menu_int_var("up_left_neighbor", i - r_offset - 1);
						push_menu_int_var("up_right_neighbor", i - r_offset);
					}
					else {
						push_menu_int_var("up_left_neighbor", i - r_offset);
						push_menu_int_var("up_right_neighbor", i - r_offset);
					}
				}
				if (my_row == num_rows - 1) {
					push_menu_int_var("down_left_neighbor", -1);
					push_menu_int_var("down_right_neighbor", -1);
					if ((int)my_col == l_offset) {
						push_menu_int_var("left_neighbor", -1);
						push_menu_int_var("right_neighbor", i + 1);
					}
					else if ((int)my_col == num_cols_bottom_row) {
						push_menu_int_var("left_neighbor", i - 1);
						push_menu_int_var("right_neighbor", -1);
					}
					else {
						push_menu_int_var("left_neighbor", i - 1);
						push_menu_int_var("right_neighbor", i + 1);
					}
				}
				else {
					if (my_row == num_rows - 2) {
						if (my_col <= l_offset) {
							push_menu_int_var("down_left_neighbor", (my_row + 1) * 10);
							push_menu_int_var("down_right_neighbor", (my_row + 1) * 10);
						}
						else if (my_col >= r_offset) {
							push_menu_int_var("down_left_neighbor", (my_row + 1) * 10 + num_cols_bottom_row - 1);
							push_menu_int_var("down_right_neighbor", (my_row + 1) * 10 + num_cols_bottom_row - 1);
						}
						else {
							if (diagonal_neighbors) {
								push_menu_int_var("down_left_neighbor", i + r_offset);
								push_menu_int_var("down_right_neighbor", i + r_offset + 1);
							}
							else {
								push_menu_int_var("down_left_neighbor", i + r_offset);
								push_menu_int_var("down_right_neighbor", i + r_offset);
							}
						}
					}
					else {
						push_menu_int_var("down_left_neighbor", i + num_cols);
						push_menu_int_var("down_right_neighbor", i + num_cols);
					}
					if ((int)my_col == 0) {
						push_menu_int_var("left_neighbor", -1);
						push_menu_int_var("right_neighbor", i + 1);
					}
					else if ((int)my_col == num_cols - 1) {
						push_menu_int_var("left_neighbor", i - 1);
						push_menu_int_var("right_neighbor", -1);
					}
					else {
						push_menu_int_var("left_neighbor", i - 1);
						push_menu_int_var("right_neighbor", i + 1);
					}
				}

				push_menu_texture(
					"Chara Render",
					"resource/game_state/chara_select/chara/" + resource_name + "/render.png"
				);
				last_pushed_texture->set_scale(0.5f);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
				last_pushed_texture->set_pos(glm::vec3(
					my_col * (last_pushed_texture->get_width() * 2 + 2) - 3000,
					2500 - (float)my_row * (last_pushed_texture->get_height() * 2 + 3),
					0
				));
				last_pushed_texture->set_target_pos(glm::vec3(
					my_col * (last_pushed_texture->get_width() * 2 + 2) + 1092,
					490 - (float)my_row * (last_pushed_texture->get_height() * 2 + 3),
					0
				), my_col * 5 + my_row * 2 + 8);
			} pop_menu_stack();
		}
	} pop_menu_stack();
	
	return true;
}

void CSS::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();
	if (thread_manager->is_active(THREAD_KIND_LOAD)) {
		if (thread_loaded_chars > loaded_chars) {
			load_chara_model_into_main_thread();
		}
		else if (loaded_chars == get_menu_object("Chara Slots").int_var("num_slots")) {
			thread_manager->kill_thread(THREAD_KIND_LOAD);
		}
	}
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
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
		if (demo_models[i].anim_kind == nullptr) {
			demo_models[i].rot = glm::vec3(glm::radians(-90.0), 0.0, glm::radians(90.0 * (i * -2 + 1)));
		}
		else {
			demo_models[i].rot = glm::vec3(0.0, 0.0, glm::radians(90.0));
		}
		int selection = get_menu_object("Player Cursors").get_child(i).int_var("selected_slot");
		if (selection >= 0 && selection < loaded_chars) {
			demo_models[i].animate();
			if (demo_models[i].anim_end) {
				//is_anim_end will always be false when anim_kind is nullptr, so we don't
				//need to worry about this
				if (demo_models[i].anim_kind->name == "selected") {
					demo_models[i].change_anim("selected_wait", 1.0f, 0.0f);
					demo_models[i].animate();
				}
				else if (demo_models[i].anim_kind->name == "deselected") {
					demo_models[i].change_anim("deselected_wait", 1.0f, 0.0f);
					demo_models[i].animate();
				}
			}
			demo_models[i].render_shadow();
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
		int selection = get_menu_object("Player Cursors").get_child(i).int_var("selected_slot");
		if (selection >= 0 && selection < loaded_chars) {
			demo_models[i].render();
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	render_menu_object("Background");
	glViewport(render_manager->res_width * 0.2, render_manager->res_height * 0.34, render_manager->res_width * 0.6, render_manager->res_height * 0.6);
	render_manager->g_buffer.render();
	glViewport(0, 0, render_manager->res_width, render_manager->res_height);

	glDepthMask(GL_FALSE);
	render_menu_object("Foreground");
	render_menu_object("Chara Slots");
	render_menu_object("Player Cursors");
}

void CSS::event_select_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_select_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_select_press();
	}
}
void CSS::event_back_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller() && 
		get_menu_object("Player Cursors").get_child((bool)!player_id).int_var("chara_selection_state") % 2) {
		//All of the hovering states happen to be even, and those are the ones for which we don't want
		//to use p2's events, so we can get away with this lol
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_back_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_back_press();
	}
}

void CSS::event_start_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state") == CHARA_SELECTION_STATE_READY
	&& get_menu_object("Player Cursors").get_child((bool)!player_id).int_var("chara_selection_state") == CHARA_SELECTION_STATE_READY) {
		for (int i = 0; i < 2; i++) {
			MenuObject& cursor = get_menu_object("Player Cursors").get_child(i);
			player[i]->chara_kind = get_menu_object("Chara Slots")
				.get_child(cursor.int_var("selected_slot")).int_var("chara_kind");
			player[i]->alt_costume = cursor.int_var("selected_costume");
			player[i]->alt_color = cursor.int_var("selected_color");
		}
		update_state(GAME_STATE_BATTLE);
		return;
	}
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
	== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_start_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_start_press();
	}
}

void CSS::event_right_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_right_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_right_press();
	}
}

void CSS::event_left_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_left_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_left_press();
	}
}

void CSS::event_down_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_down_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_down_press();
	}
}

void CSS::event_up_press() {
	if (get_menu_object("Player Cursors").get_child(player_id).int_var("chara_selection_state")
		== CHARA_SELECTION_STATE_READY && !player[!player_id]->controller.has_any_controller()) {
		get_menu_object("Player Cursors").get_child((bool)!player_id).event_up_press();
	}
	else {
		get_menu_object("Player Cursors").get_child(player_id).event_up_press();
	}
}

/// <summary>
/// Select the CSS slot that the cursor is hovering over. Works by comparing the column and row of each slot to the column and row of the player, so 
/// make sure to set that up properly if this function is copied into another menu.
/// </summary>
void CSS::select_slot(int player_idx) {
	MenuObject& cursor = get_menu_object("Player Cursors").get_child(player_idx);
	MenuObject& chara_slot = get_menu_object("Chara Slots").get_child(cursor.int_var("selected_slot"));

	cursor.get_texture("Cursor").set_target_pos(chara_slot.get_texture("Chara Render").pos, 8);

	cursor.int_var("selected_costume") = player[player_idx]->player_info->preferred_costume[chara_slot.int_var("chara_kind")];
	cursor.int_var("selected_color") = player[player_idx]->player_info->preferred_color[chara_slot.int_var("chara_kind")];

	GameTexture& large_css_slot = cursor.get_texture("Chara Slot Large");

	large_css_slot = GameTexture(chara_slot.get_texture("Chara Render"));
	large_css_slot.set_scale(1.25f);
	if (player_idx) {
		large_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
	}
	else {
		large_css_slot.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	large_css_slot.set_pos(glm::vec3(130.5, 338, 0));
	large_css_slot.set_alpha((Uint8)127);

	if (demo_models[player_idx].model.is_loaded()) {
		demo_models[player_idx].model.unload_model_instance();
	}
	if (cursor.int_var("selected_slot") < loaded_chars) {
		demo_models[player_idx].load_model(
			"resource/chara/" + chara_slot.string_var("resource_name") +
			"/model/m" + std::to_string(cursor.int_var("selected_costume")) + "/model.dae",
			"c" + std::to_string(cursor.int_var("selected_color"))
		);
		demo_models[player_idx].anim_table = demo_anim_tables[cursor.int_var("selected_slot")];
		if (cursor.int_var("chara_selection_state") == CHARA_SELECTION_STATE_CHARA_HOVER) {
			demo_models[player_idx].change_anim("deselected_wait", 1.0f, 0.0f);
		}
		else {
			demo_models[player_idx].change_anim("selected_wait", 1.0f, 0.0f);
		}
	}
}

void CSS::select_costume(int player_idx) {
	if (demo_models[player_idx].model.is_loaded()) {
		demo_models[player_idx].model.unload_model_instance();
	}
	MenuObject& player_cursor = get_menu_object("Player Cursors").get_child(player_idx);
	MenuObject& selected_slot = get_menu_object("Chara Slots").get_child(player_cursor.int_var("selected_slot"));
	demo_models[player_idx].load_model(
		"resource/chara/" + selected_slot.string_var("resource_name") +
		"/model/m" + std::to_string(player_cursor.int_var("selected_costume")) + "/model.dae",
		"c0"
	);
}

void CSS::select_color(int player_idx) {
	if (demo_models[player_idx].model.is_loaded()) {
		demo_models[player_idx].model.unload_textures();
		demo_models[player_idx].model.load_textures("c" +
			std::to_string(get_menu_object("Player Cursors").get_child(player_idx).int_var("selected_color"))
		);
	}
}

/// <summary>
/// Takes the character that the current player is already selecting, and automatically puts their cursor on the first CSS slot corresponding to that
/// character. Note: This function is used for when you re-enter the CSS after having already selected a character, such as after a match.
/// </summary>
/// <param name="chara_kind">: The character whose CSS slot to select.</param>
void CSS::select_default_chara_kind(int chara_kind) {
	MenuObject& player_cursor = get_menu_object("Player Cursors").get_child(player_id);
	for (int i = 0, max = get_menu_object("Chara Slots").int_var("num_slots"); i < max; i++) {
		if (get_menu_object("Chara Slots").get_child(i).int_var("chara_kind") == chara_kind) {
			player_cursor.int_var("selected_slot") = i;
			select_slot(player_id);
			return;
		}
	}
}

void CSS::load_chara_model_into_main_thread() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	resource_manager->init_gl_model("resource/chara/"
		+ get_menu_object("Chara Slots").get_child(loaded_chars).string_var("resource_name")
		+ "/model/m0/model.dae"
	);

	for (int i = 0; i < 2; i++) {
		MenuObject& cursor = get_menu_object("Player Cursors").get_child(i);
		int selection = cursor.int_var("selected_slot");
		if (loaded_chars == selection && !demo_models[i].model.is_loaded()) {
			demo_models[i].load_model(
				"resource/chara/"
				+ get_menu_object("Chara Slots").get_child(selection).string_var("resource_name")
				+ "/model/m" + std::to_string(cursor.int_var("selected_costume")) + "/model.dae",
				"c" + std::to_string(cursor.int_var("selected_color"))
			);
			demo_models[i].init_shader();
			demo_models[i].anim_table = demo_anim_tables[selection];
			if (cursor.int_var("chara_selection_state") == CHARA_SELECTION_STATE_CHARA_HOVER) {
				demo_models[i].change_anim("deselected_wait", 1.0f, 0.0f);
			}
			else {
				demo_models[i].change_anim("selected_wait", 1.0f, 0.0f);
			}
			demo_models[i].model.set_flip(i);
		}
	}

	loaded_chars++;
}