#include "CharaSelect.h"
#include "GameManager.h"
#include "FontManager.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "ThreadManager.h"
#include "GLM Helpers.h"
#include "PlayerInfo.h"
#include "debug.h"
#include <fstream>
#include "WindowConstants.h"
#include "SaveManager.h"
#include "ShaderManager.h"

/// <summary>
/// The main function while on the character select screen.
/// </summary>
void chara_select_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();

	window_manager->reset_gl_environment();

	CSS *css = new CSS;

	window_manager->update_shader_cams();
	window_manager->update_shader_lights();

	while (css->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();
		
		css->process();
		css->render();

		window_manager->update_screen();
	}

	delete css;
}

CSS::CSS() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();

	loaded_chars = 0;
	thread_loaded_chars = 0;

	stage_demo.load_model(player[0]->stage_info.resource_dir + "assets/main/model/model.dae");
	stage_demo.init_shader();
	stage_demo.model.load_textures();

	lights.reserve(MAX_LIGHT_SOURCES);
	std::ifstream light_stream;
	light_stream.open(player[0]->stage_info.resource_dir + "/assets/main/param/lights.yml");
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
		lights.push_back(window_manager->add_light(light_pos, light_col, brightness));
	}
	light_stream.close();

	load_font("Name Entry Font", "FiraCode", 46);

	root.add_elements({ 
		{"Background", SceneElement({
			{"bg", ScreenTexture("resource/scene/chara_select/background.png")}
		})},
		{"Foreground", SceneElement({
			{"chara_deck", ScreenTexture("resource/scene/chara_select/chara_deck.png")
				.set_pos(glm::vec3(0.0f, -2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"top_table", ScreenTexture("resource/scene/chara_select/top_table.png")
				.set_pos(glm::vec3(0.0f, 2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"frame_l", ScreenTexture("resource/scene/chara_select/frame_l.png")
				.set_pos(glm::vec3(-2000.0f, 0.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"frame_r", ScreenTexture("resource/scene/chara_select/frame_r.png")
				.set_pos(glm::vec3(2000.0f, 0.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"panel_dl", ScreenTexture("resource/scene/chara_select/panel_dl.png")
				.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"panel_dr", ScreenTexture("resource/scene/chara_select/panel_dr.png")
				.set_pos(glm::vec3(2000.0f, -2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"panel_ul", ScreenTexture("resource/scene/chara_select/panel_ul.png")
				.set_pos(glm::vec3(2000.0f, -2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
			{"panel_ur", ScreenTexture("resource/scene/chara_select/panel_ur.png")
				.set_pos(glm::vec3(2000.0f, 2000.0f, 0.0f))
				.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
			},
		})}
	});

	if (!load_css()) {
		game_manager->add_crash_log("Could not load CSS!");
		return;
	}

	ThreadManager* thread_manager = ThreadManager::get_instance();
	thread_manager->add_thread(THREAD_KIND_LOAD, gamestate_charaselect_loading_thread, this);
	thread_manager->notify_thread(THREAD_KIND_LOAD);

	SaveManager* save_manager = SaveManager::get_instance();

	root.add_element("Player Cursors", SceneElement());
	load_event("Move Mobile Slots", [this](SceneElement* e) {
		ScreenTexture& mobile_css_slot = e->get_parent().get_screen_texture("Chara Slot Mobile");
		ScreenTexture& large_css_slot = e->get_parent().get_screen_texture("Chara Slot Large");
		if (mobile_css_slot.get_pos() != large_css_slot.get_pos()) {
			mobile_css_slot.add_rot(glm::vec3(0.0, 360.0 / 16.0, 0.0));
			mobile_css_slot.add_width_scale(0.7 / 16);
			mobile_css_slot.add_height_scale(0.7 / 16);
		}
		else {
			mobile_css_slot.set_alpha(0);
			mobile_css_slot.set_rot(glm::vec3(0.0, 0.0, 0.0));
			large_css_slot.set_alpha(255);
		}
	});
	load_event("Hover Back Press", [this](SceneElement* e) {
		if (!player[player_id]->controller.has_any_controller()) {
			player_id = (bool)!player_id;
			set_player_active_element(&e->get_sibling("Chara"));
			execute_event("deactivate_from_ready");
			player_id = (bool)!player_id;
		}
		else {
			ThreadManager* thread_manager = ThreadManager::get_instance();
			if (thread_manager->is_active(THREAD_KIND_LOAD)) {
				thread_manager->kill_thread(THREAD_KIND_LOAD);
			}
			update_scene(SCENE_STAGE_SELECT, SCENE_CONTEXT_NONE);
		}
	});

	for (int i = 0; i < 2; i++) {
		root.get_child("Player Cursors").add_element("P" + std::to_string(i + 1) + " Cursor",
			SceneElement({
				{"Name Entry Hover", SceneElement()
					.add_event("activate", [this](SceneElement* e) {
						SceneElement& parent = e->get_parent();
						if (!player_id) {
							parent.get_screen_texture("Cursor").set_pos(glm::vec3(125, 168.5, 0), 12);
						}
						else {
							parent.get_screen_texture("Cursor").set_pos(glm::vec3(3555, 168.5, 0), 12);
						}
						parent.get_screen_texture("Name Entry Cursor").set_alpha(255);
					})
					.add_event("process", [this](SceneElement* e) {
						e->get_parent().get_screen_texture("Name Entry Cursor").next_sprite();
					})
					.add_event("up_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Controls Hover"));
					})
					.add_event("select_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Name Entry"));
					})
					.add_event("back_press", get_event("Hover Back Press"))
				},
				{"Name Entry", SceneElement()
					.add_event("activate", [this](SceneElement* e) {
						SceneElement& parent = e->get_parent();
						SaveManager* save_manager = SaveManager::get_instance();
						int num_entries = save_manager->get_num_player_info();
						for (int i2 = 0, max2 = std::min(5, num_entries + 1);
							i2 < max2; i2++) {
							parent.get_screen_texture("Name Entry " + std::to_string(i2)).set_pos(glm::vec3(128, 338.5 - 64 * (i2 + 1), 0.0f), 5);
							parent.get_screen_text("Name Entry Text " + std::to_string(i2)).set_pos(glm::vec3(1580 * (player_id * 2 - 1), 340 - 64 * (i2 + 1), 0.0f), 5);
						}
						parent.get_screen_text("Name Entry Text").update_text("Player " + std::to_string(player_id + 1));
						int idx = parent.int_var("name_entry_idx");
						if (num_entries >= 5 && idx > 2) {
							idx = std::max(4 - (num_entries - idx), 2);
						}
						parent.get_screen_texture("Name Entry Cursor").set_pos(glm::vec3(128, 338.5 - 64 * (idx + 1), 0.0f), 5);
					})
					.add_event("deactivate", [this](SceneElement* e) {
						SceneElement& parent = e->get_parent();
						SaveManager* save_manager = SaveManager::get_instance();
						int num_entries = save_manager->get_num_player_info();
						for (int i2 = 0, max2 = std::min(5, num_entries + 1);
							i2 < max2; i2++) {
							parent.get_screen_texture("Name Entry " + std::to_string(i2)).set_pos(glm::vec3(128, 338.5, 0.0f), 5);
							parent.get_screen_texture("Name Entry Text " + std::to_string(i2)).set_pos(glm::vec3(1580 * (player_id * 2 - 1), 340, 0.0f), 5);
						}
						if (parent.int_var("name_entry_idx") == -1) {
							parent.get_screen_text("Name Entry Text").update_text("Player " + std::to_string(player_id + 1));
						}
						else {
							parent.get_screen_text("Name Entry Text").update_text(save_manager->get_player_info(parent.int_var("name_entry_idx"))->name);
						}
						parent.get_screen_texture("Name Entry Cursor").set_pos(glm::vec3(128, 338.5, 0.0f), 5);
					})
					.add_event("process", [this](SceneElement* e) {
						SceneElement& p = e->get_parent();
						SaveManager* save_manager = SaveManager::get_instance();
						int num_entries = save_manager->get_num_player_info();
						if (num_entries < 5) {
							int i2;
							for (i2 = 0; i2 < num_entries; i2++) {
								p.get_screen_text("Name Entry Text " + std::to_string(i2)).update_text(save_manager->get_player_info(i2)->name);
							}
							p.get_screen_text("Name Entry Text " + std::to_string(i2)).update_text("New Name");
						}
						else if (p.int_var("name_entry_idx") < 3) {
							for (int i2 = 0; i2 < 5; i2++) {
								p.get_screen_text("Name Entry Text " + std::to_string(i2)).update_text(save_manager->get_player_info(i2)->name);
							}
						}
						else if (num_entries - p.int_var("name_entry_idx") < 3) {
							int idx = 0;
							for (int i2 = num_entries - 4; i2 < num_entries; i2++, idx++) {
								p.get_screen_text("Name Entry Text " + std::to_string(idx)).update_text(save_manager->get_player_info(i2)->name);
							}
							p.get_screen_text("Name Entry Text 4").update_text("New Name");
						}
						else {
							int idx = 0;
							for (int i2 = p.int_var("name_entry_idx") - 2; i2 < p.int_var("name_entry_idx") + 3; i2++, idx++) {
								p.get_screen_text("Name Entry Text " + std::to_string(idx)).update_text(save_manager->get_player_info(i2)->name);
							}
						}
						p.get_screen_texture("Name Entry Cursor").next_sprite();
					})
					.add_event("up_press", [this](SceneElement* e) {
						SaveManager* save_manager = SaveManager::get_instance();
						int num_entries = save_manager->get_num_player_info();
						if (e->int_var("name_entry_idx") == -1) return;
						if (e->int_var("name_entry_idx") < 3
						|| num_entries - e->int_var("name_entry_idx") < 2
						|| num_entries <= 5) {
							TargetVar<glm::vec3> target = e->get_screen_texture("Name Entry Cursor").get_pos_target();
							if (target.get_frames()) {
								glm::vec3 target_pos = target.get_target_val();
								e->get_screen_texture("Name Entry Cursor").set_pos(target_pos + glm::vec3(0.0, 64.0, 0.0), target.get_frames());
							}
							else {
								e->get_screen_texture("Name Entry Cursor").add_pos(glm::vec3(0.0, 64.0, 0.0));
							}
						}
						e->int_var("name_entry_idx")--;
					})
					.add_event("down_press", [this](SceneElement* e) {
						SaveManager* save_manager = SaveManager::get_instance();
						int num_entries = save_manager->get_num_player_info();
						if (e->int_var("name_entry_idx") == num_entries) return;
							if (e->int_var("name_entry_idx") < 2
							|| num_entries - e->int_var("name_entry_idx") < 3
							|| num_entries <= 5) {
								TargetVar<glm::vec3> target = e->get_screen_texture("Name Entry Cursor").get_pos_target();
								if (target.get_frames()) {
									glm::vec3 target_pos = target.get_target_val();
									e->get_screen_texture("Name Entry Cursor").set_pos(target_pos - glm::vec3(0.0, 64.0, 0.0), target.get_frames());
								}
								else {
									e->get_screen_texture("Name Entry Cursor").add_pos(glm::vec3(0.0, -64.0, 0.0));
								}
							}
							e->int_var("name_entry_idx")++;
					})
					.add_event("left_press", [this](SceneElement* e) {
						if (player_id) {
							e->int_var("selected_slot") = e->int_var("prev_selected_slot");
							set_player_active_element(&e->get_sibling("Chara Hover"));
						}
					})
					.add_event("right_press", [this](SceneElement* e) {
						if (!player_id) {
							e->int_var("selected_slot") = e->int_var("prev_selected_slot");
							set_player_active_element(&e->get_sibling("Chara Hover"));
						}
					})
					.add_event("select_press", [this](SceneElement* e) {
						SaveManager* save_manager = SaveManager::get_instance();
						if (e->int_var("name_entry_idx") == save_manager->get_num_player_info()) {
							//We hit the "New Player Info" button, time to do some shenanigans
							return;
						}
						e->int_var("prev_name_entry_idx") = e->int_var("name_entry_idx");
						player[player_id]->load_player_info(e->int_var("name_entry_idx"));
						select_preferred_chara_kind(player[player_id]->player_info);
						player[player_id]->controller.poll_menu();
						set_player_active_element(&e->get_sibling("Chara Hover"));
					})
					.add_event("back_press", [this](SceneElement* e) {
						e->int_var("name_entry_idx") = e->int_var("prev_name_entry_idx");
						set_player_active_element(&e->get_sibling("Name Entry Hover"));
					})
				},
				{"Controls Hover", SceneElement()
					.add_event("down_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Name Entry Hover"));
					})
					.add_event("select_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Controls"));
					})
					.add_event("back_press", get_event("Hover Back Press"))
				},
				{"Controls", SceneElement()
					.add_event("start_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Controls Hover"));
					})
				},
				{"Chara Hover", SceneElement()
					.add_event("activate", [this](SceneElement* e) {
						SceneElement& parent = e->get_parent();
						parent.get_screen_texture("Chara Slot Large").set_alpha(127);
						parent.get_screen_texture("Name Entry Cursor").set_alpha(0);
						select_slot(player_id);
					})
					.add_event("up_press", [this](SceneElement* e) {
						std::string neighbor_val = "up_left_neighbor";
						if (e->bool_var("last_input_right")) {
							neighbor_val = "up_right_neighbor";
						}
						int new_selection = root.get_child("Chara Slots")
							.get_child(e->int_var("selected_slot")).int_var(neighbor_val);
						if (new_selection == -1) return;
						e->int_var("prev_selected_slot") = e->int_var("selected_slot");
						e->int_var("selected_slot") = new_selection;
						e->bool_var("last_input_right") = !e->bool_var("last_input_right");
						select_slot(player_id);
					})
					.add_event("down_press", [this](SceneElement* e) {
						std::string neighbor_val = "down_left_neighbor";
						if (e->bool_var("last_input_right")) {
							neighbor_val = "down_right_neighbor";
						}
						int new_selection = root.get_child("Chara Slots")
							.get_child(e->int_var("selected_slot")).int_var(neighbor_val);
						if (new_selection == -1) return;
						e->int_var("prev_selected_slot") = e->int_var("selected_slot");
						e->int_var("selected_slot") = new_selection;
						e->bool_var("last_input_right") = !e->bool_var("last_input_right");
						select_slot(player_id);
					})
					.add_event("left_press", [this](SceneElement* e) {
						int new_selection = root.get_child("Chara Slots")
							.get_child(e->int_var("selected_slot")).int_var("left_neighbor");
						if (new_selection == -1) {
							if (!player_id) {
								e->int_var("prev_selected_slot") = e->int_var("selected_slot");
								e->int_var("selected_slot") = new_selection;
								e->bool_var("last_input_right") = false;
								set_player_active_element(&e->get_sibling("Name Entry Hover"));
							}
							return;
						}
						e->int_var("prev_selected_slot") = e->int_var("selected_slot");
						e->int_var("selected_slot") = new_selection;
						e->bool_var("last_input_right") = false;
						select_slot(player_id);
					})
					.add_event("right_press", [this](SceneElement* e) {
						int new_selection = root.get_child("Chara Slots")
							.get_child(e->int_var("selected_slot")).int_var("right_neighbor");
						if (new_selection == -1) {
							if (player_id) {
								e->int_var("prev_selected_slot") = e->int_var("selected_slot");
								e->int_var("selected_slot") = new_selection;
								e->bool_var("last_input_right") = true;
								set_player_active_element(&e->get_sibling("Name Entry Hover"));
							}
							return;
						}
						e->int_var("prev_selected_slot") = e->int_var("selected_slot");
						e->int_var("selected_slot") = new_selection;
						e->bool_var("last_input_right") = true;
						select_slot(player_id);
					})
					.add_event("select_press", [this](SceneElement* e) {
						demo_models[player_id].change_anim("selected", 1.0f, 0.0f);
						set_player_active_element(&e->get_sibling("Chara"));
						execute_event("activate_from_hover");
					})
					.add_event("back_press", get_event("Hover Back Press"))
				},
				{"Chara", SceneElement()
					.add_event("activate_from_hover", [this](SceneElement* e) {
						//This is NOT an activation event, this event should be called during Chara Hover's 
						//"select_press" event
						SceneElement& p = e->get_parent();
						ScreenTexture& mobile_css_slot = p.get_screen_texture("Chara Slot Mobile");
						ScreenTexture& chara_render = root.get_child("Chara Slots")
							.get_child(p.int_var("selected_slot"))
							.get_screen_texture("Chara Render");

						mobile_css_slot = ScreenTexture(chara_render);
						mobile_css_slot.set_pos(p.get_screen_texture("Chara Slot Large").get_pos(), 16);
					})
					.add_event("deactivate_from_ready", [this](SceneElement* e) {
						e->get_parent().get_screen_texture("Chara Slot Mobile").set_alpha(0);
					})
					.add_event("process", get_event("Move Mobile Slots"))
					.add_event("up_press", [this](SceneElement* e) {
						if (e->int_var("selected_costume") == 0) {
								e->int_var("selected_costume") = root.get_child("Chara Slots")
								.get_child(e->int_var("selected_slot")).int_var("num_costumes") - 1;
						}
						else {
							e->int_var("selected_costume")--;
						}
						select_costume(player_id);
					})
					.add_event("down_press", [this](SceneElement* e) {
						if (e->int_var("selected_costume") + 1 == root.get_child("Chara Slots")
								.get_child(e->int_var("selected_slot")).int_var("num_costumes")) {
							e->int_var("selected_costume") = 0;
						}
						else {
							e->int_var("selected_costume")++;
						}
						select_costume(player_id);
					})
					.add_event("left_press", [this](SceneElement* e) {
						SceneElement& chara_slot = root.get_child("Chara Slots").get_child(e->int_var("selected_slot"));
						if (e->int_var("selected_color") == 0) {
							e->int_var("selected_color") = chara_slot.int_var("costume_" +
							std::to_string(e->int_var("selected_costume")) + "_num_colors") - 1;
						}
						else {
							e->int_var("selected_color")--;
						}
						select_color(player_id);
					})
					.add_event("right_press", [this](SceneElement* e) {
						SceneElement& chara_slot = root.get_child("Chara Slots").get_child(e->int_var("selected_slot"));
						if (e->int_var("selected_color") + 1 == chara_slot
							.int_var("costume_" + std::to_string(e->int_var("selected_costume"))
								+ "_num_colors")) {
							e->int_var("selected_color") = 0;
						}
						else {
							e->int_var("selected_color")++;
						}
						select_color(player_id);
					})
					.add_event("select_press", [this](SceneElement* e) {
						set_player_active_element(&e->get_sibling("Ready"));
					})
					.add_event("back_press", [this](SceneElement* e) {
						demo_models[player_id].change_anim("deselected", 1.0f, 0.0f);
						set_player_active_element(&e->get_sibling("Chara Hover"));
					})
				},
				{"Ready", SceneElement()
					.add_event("process", get_event("Move Mobile Slots"))
					.add_event("up_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("up_press");
							player_id = (bool)!player_id;
						}
					})
					.add_event("down_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("down_press");
							player_id = (bool)!player_id;
						}
					})
					.add_event("left_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("left_press");
							player_id = (bool)!player_id;
						}
					})
					.add_event("right_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("right_press");
							player_id = (bool)!player_id;
						}
					})
					.add_event("select_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("select_press");
							player_id = (bool)!player_id;
						}
					})
					.add_event("back_press", [this](SceneElement* e) {
						if (!player[!player_id]->controller.has_any_controller()) {
							player_id = (bool)!player_id;
							execute_event("back_press");
							player_id = (bool)!player_id;
						}
						else {
							set_player_active_element(&e->get_sibling("Chara"));
							execute_event("deactivate_from_ready");
						}
					})
					.add_event("start_press", [this](SceneElement* e) {
						player_id = (bool)!player_id;
						bool other_player_ready = (&get_active_element() == &root.get_child("P" + std::to_string(player_id + 1) + " Cursor/Ready"));
						if (other_player_ready) {
							execute_event("confirm_chara");
							player_id = (bool)!player_id;
							execute_event("confirm_chara");
							if (context == SCENE_CONTEXT_NONE || context == SCENE_CONTEXT_SPECIAL) {
								if (player[!player_id]->controller.has_any_controller()) {
									player[!player_id]->player_kind = PLAYER_KIND_PLAYER;
								}
								else {
									player[!player_id]->player_kind = PLAYER_KIND_CPU;
								}
							}
							player[player_id]->player_kind = PLAYER_KIND_PLAYER;
							update_scene(SCENE_BATTLE, SCENE_CONTEXT_NONE);
						}
						else {
							player_id = (bool)!player_id;
						}
					})
					.add_event("confirm_chara", [this](SceneElement* e) {
						SceneElement& p = e->get_parent();
						player[player_id]->chara_kind = root.get_child("Chara Slots")
							.get_child(p.int_var("selected_slot")).int_var("chara_kind");
						player[player_id]->alt_costume = p.int_var("selected_costume");
						player[player_id]->alt_color = p.int_var("selected_color");
						player[player_id]->update_player_info();
					})
				},
				{"Name Entry Loop", SceneElementLoop(5, [this, i](SceneElement* e, int idx) {
					e->add_elements({
						{"Name Entry " + std::to_string(idx), ScreenTexture("resource/scene/chara_select/name_entry.png")
							.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
							.set_pos(glm::vec3(128, 338.5, 0.0f), 16)
							.set_orientation(i ? TEXTURE_BOTTOM_RIGHT : TEXTURE_BOTTOM_LEFT)
						},
						{"Name Entry Text " + std::to_string(idx), ScreenText(&get_font("Name Entry Font"), "Name Entry", TextSpecifier().color(glm::vec3(0.0f)))
							.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
							.set_pos(glm::vec3(1580 * (i * 2 - 1), 340, 0.0f), 16)
							.set_orientation(TEXTURE_BOTTOM)
						}
					});
				})},
				{"Name Entry", ScreenTexture("resource/scene/chara_select/name_entry.png")
					.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
					.set_pos(glm::vec3(128, 338.5, 0.0f), 16)
					.set_orientation(i ? TEXTURE_BOTTOM_RIGHT : TEXTURE_BOTTOM_LEFT)
				},
				{"Name Entry Text", ScreenText(&get_font("Name Entry Font"), 
					(player[i]->player_info == save_manager->get_player_info(-1)) 
					? "Player " + std::to_string(i + 1): player[i]->player_info->name,
					TextSpecifier().color(glm::vec3(0.0f)))
					.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
					.set_pos(glm::vec3(1580 * (i * 2 - 1), 340, 0.0f), 16)
					.set_orientation(TEXTURE_BOTTOM)
				},
				{"Name Entry Cursor", ScreenTexture("resource/scene/chara_select/name_entry_cursor.gif")
					.set_pos(glm::vec3(-2000.0f, -2000.0f, 0.0f))
					.set_pos(glm::vec3(128, 338.5, 0.0f), 16)
					.set_alpha(0)
					.set_orientation(i ? TEXTURE_BOTTOM_RIGHT : TEXTURE_BOTTOM_LEFT)
				},
				{"Chara Slot Mobile", ScreenTexture()},
				{"Chara Slot Large", ScreenTexture(root.get_screen_texture("Chara Slots/chara_slot_rowan/Chara Render"))
					.set_scale(1.25f)
					.set_pos(glm::vec3(i ? 3709.5f : 103.5f, 418.0f,0))
					.set_orientation(TEXTURE_BOTTOM_LEFT)
					.set_alpha(127)
				},
				{"Cursor", ScreenTexture("resource/scene/chara_select/p" + std::to_string(i + 1) + 
					"_cursor.png")
					.set_orientation(TEXTURE_BOTTOM_LEFT)
					.set_pos(root.get_screen_texture("Chara Slots/chara_slot_rowan/Chara Render").get_pos_target().get_target_val(), 16)
				}
			})
			.push_int_var("name_entry_idx", -1)
			.push_int_var("prev_name_entry_idx", -1)
			.push_int_var("selected_slot", 0)
			.push_int_var("selected_costume", 0)
			.push_int_var("prev_selected_slot", 0)
			.push_bool_var("last_input_right", true)
		);
		if (player[i]->player_info != save_manager->get_player_info(-1)) {
			root.get_child("Player Cursors/P" + std::to_string(i + 1) + " Cursor").int_var("name_entry_idx")
				= save_manager->add_player_info(player[i]->player_info->name);
		}
	}

	root.get_child("Player Cursors").add_elements({
		{"slot_hover_l", ScreenTexture("resource/scene/chara_select/slot_hover_l.png")
			.set_pos(glm::vec3(-2000.0f, 0.0f, 0.0f))
			.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
		},
		{"slot_hover_r", ScreenTexture("resource/scene/chara_select/slot_hover_r.png")
			.set_pos(glm::vec3(-2000.0f, 0.0f, 0.0f))
			.set_pos(glm::vec3(0.0f, 0.0f, 0.0f), 16)
		}
	});

	for (int i = 0; i < 2; i++) {
		player_id = i;
		select_preferred_chara_kind(player[i]->player_info);
		select_slot(i);
		demo_models[i].set_rot(glm::vec3(0.0, 0.0, 90.0));
		if (!i) {
			demo_models[i].set_pos(glm::vec3(-350.0, 0.0, 0.0));
		}
		else {
			demo_models[i].set_pos(glm::vec3(350.0, 0.0, 0.0));
		}
	}
}

void gamestate_charaselect_loading_thread(void* charaselect_arg) {
	CSS* css = (CSS*)charaselect_arg;
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = css->root.get_child("Chara Slots").get_num_children(); i < max; i++) {
		SceneElement& chara_slot = css->root.get_child("Chara Slots").get_child(i);
		std::string resource_dir = "resource/chara/" + chara_slot.string_var("resource_name");
		ModelData* model_data = ResourceManager::get_instance()->get_model_keep_user_count(resource_dir + "/model/m0/model.fbx");
		css->demo_anim_tables.push_back(AnimationTable());
		if (model_data->is_loaded()) {
			css->demo_anim_tables.back().load_anlst(resource_dir + "/anims/demo", model_data->skeleton);
		}
		css->thread_loaded_chars++;
	}
}

CSS::~CSS() {
	WindowManager* window_manager = WindowManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	window_manager->remove_lights();
	resource_manager->unload_unused();
}

/// <summary>
/// Loads the CSS file and sets up all of the slots.
/// </summary>
/// <returns>true if successful, false if the file fails to open.</returns>
bool CSS::load_css() {
	ParamTable css_params("resource/scene/chara_select/css_param.prmlst");
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

	root.add_element("Chara Slots",
		SceneElement({
			//TODO: The following capture by reference feels really dangerous. I'm not sure if it'd 
			//actually break the memory but I wouldn't be very surprised. If there are any weird CSS
			//crashes while loading, this would be a good place to look
			{"Loop", SceneElementLoop(num_slots, [&](SceneElement* e, int i) {
				ParamTable param_table(css_params.get_param_table(i + css_param_sub_table_start));
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
				e->add_element("chara_slot_" + resource_name, SceneElement({
					{"Chara Render", ScreenTexture("resource/scene/chara_select/chara/" + resource_name + "/render.png")
						.set_scale(0.5f)
						.set_orientation(TEXTURE_BOTTOM_LEFT)
					}
					})
					.push_int_var("chara_kind", param_table.get_param_int("chara_kind"))
					.push_string_var("resource_name", resource_name)
					.push_int_var("num_costumes", num_costumes)
					.push_bool_var("selectable", 
						param_table.get_param_bool("selectable")
					//	|| save_manager->is_chara_unlocked() <-- TODO: Make this a real function lol
					)
				);
				SceneElement& slot = e->get_child("chara_slot_" + resource_name);
				if (my_row == 0) {
					slot.push_int_var("up_left_neighbor", -1);
					slot.push_int_var("up_right_neighbor", -1);
				}
				else {
					if (diagonal_neighbors) {
						slot.push_int_var("up_left_neighbor", i - r_offset - 1);
						slot.push_int_var("up_right_neighbor", i - r_offset);
					}
					else {
						slot.push_int_var("up_left_neighbor", i - r_offset);
						slot.push_int_var("up_right_neighbor", i - r_offset);
					}
				}
				if (my_row == num_rows - 1) {
					slot.push_int_var("down_left_neighbor", -1);
					slot.push_int_var("down_right_neighbor", -1);
					if ((int)my_col == l_offset) {
						slot.push_int_var("left_neighbor", -1);
						slot.push_int_var("right_neighbor", i + 1);
					}
					else if ((int)my_col == num_cols_bottom_row) {
						slot.push_int_var("left_neighbor", i - 1);
						slot.push_int_var("right_neighbor", -1);
					}
					else {
						slot.push_int_var("left_neighbor", i - 1);
						slot.push_int_var("right_neighbor", i + 1);
					}
				}
				else {
					if (my_row == num_rows - 2) {
						if (my_col <= l_offset) {
							slot.push_int_var("down_left_neighbor", (my_row + 1) * 10);
							slot.push_int_var("down_right_neighbor", (my_row + 1) * 10);
						}
						else if (my_col >= r_offset) {
							slot.push_int_var("down_left_neighbor", (my_row + 1) * 10 + num_cols_bottom_row - 1);
							slot.push_int_var("down_right_neighbor", (my_row + 1) * 10 + num_cols_bottom_row - 1);
						}
						else {
							if (diagonal_neighbors) {
								slot.push_int_var("down_left_neighbor", i + r_offset);
								slot.push_int_var("down_right_neighbor", i + r_offset + 1);
							}
							else {
								slot.push_int_var("down_left_neighbor", i + r_offset);
								slot.push_int_var("down_right_neighbor", i + r_offset);
							}
						}
					}
					else {
						slot.push_int_var("down_left_neighbor", i + num_cols);
						slot.push_int_var("down_right_neighbor", i + num_cols);
					}
					if ((int)my_col == 0) {
						slot.push_int_var("left_neighbor", -1);
						slot.push_int_var("right_neighbor", i + 1);
					}
					else if ((int)my_col == num_cols - 1) {
						slot.push_int_var("left_neighbor", i - 1);
						slot.push_int_var("right_neighbor", -1);
					}
					else {
						slot.push_int_var("left_neighbor", i - 1);
						slot.push_int_var("right_neighbor", i + 1);
					}
				}

				//(The texture references its own width and height so we have to do this)
				ScreenTexture& tex = slot.get_screen_texture("Chara Render");
				tex.set_pos(glm::vec3(
					my_col * (tex.get_width() * 2 + 2) - 3000,
					2500 - (float)my_row * (tex.get_height() * 2 + 3),
					0
				))
				.set_pos(glm::vec3(
					my_col * (tex.get_width() * 2 + 2) + 1092,
					490 - (float)my_row * (tex.get_height() * 2 + 3),
					0
				), my_col * 5 + my_row * 2 + 8);
			})}
		})
		.push_int_var("num_slots", num_slots)
	);
	
	return true;
}

void CSS::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();
	if (thread_manager->is_active(THREAD_KIND_LOAD)) {
		//The internal_frame check is designed to hide the more obvious stutters with gl 
		//loading, and can be removed for the vulkan impl

		if (thread_loaded_chars > loaded_chars) {
			load_chara_model_into_main_thread();
		}
		else if (loaded_chars == root.get_child("Chara Slots").int_var("num_slots")) {
			thread_manager->kill_thread(THREAD_KIND_LOAD);
		}
	}
	execute_event("process");
	player_id = (bool)!player_id;
	execute_event("process");
	player_id = (bool)!player_id;
}

/// <summary>
/// Render all CSS-related textures and models
/// </summary>
void CSS::render_main() {
	WindowManager* window_manager = WindowManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	window_manager->execute_buffered_events();
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	window_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	stage_demo.render_shadow();
	for (int i = 0; i < 2; i++) {
		int selection = root.get_child("Player Cursors").get_child(i).int_var("selected_slot");
		if (selection >= 0 && selection < loaded_chars) {
			if (demo_models[i].anim_kind == nullptr) {
				demo_models[i].set_rot(glm::vec3(0.0, 0.0, 90.0 * (i * -2 + 1)));
			}
			else {
				demo_models[i].set_rot(glm::vec3(0.0, 0.0, 90.0));
			}
			demo_models[i].process_animate();
			if (demo_models[i].anim_end) {
				//is_anim_end will always be false when anim_kind is nullptr, so we don't
				//need to worry about this
				if (demo_models[i].anim_kind->name == "selected") {
					demo_models[i].change_anim("selected_wait", 1.0f, 0.0f);
					demo_models[i].process_animate();
				}
				else if (demo_models[i].anim_kind->name == "deselected") {
					demo_models[i].change_anim("deselected_wait", 1.0f, 0.0f);
					demo_models[i].process_animate();
				}
			}
			demo_models[i].render_shadow();
		}
	}
	glCullFace(GL_BACK);

	window_manager->g_buffer.use();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_manager->shadow_map.bind_textures();
	shader_manager->set_global_float("Outline", 0.5f);
	stage_demo.render();
	shader_manager->set_global_float("Outline", 1.0f);
	glDisable(GL_CULL_FACE);
	for (int i = 0; i < 2; i++) {
		int selection = root.get_child("Player Cursors").get_child(i).int_var("selected_slot");
		if (selection >= 0 && selection < loaded_chars) {
			demo_models[i].render();
		}
	}
	glEnable(GL_CULL_FACE);

	window_manager->render_ssao();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	root.get_child("Background").render();
	glViewport(window_manager->res_width * 0.2, window_manager->res_height * 0.34, window_manager->res_width * 0.6, window_manager->res_height * 0.6);
	glEnable(GL_FRAMEBUFFER_SRGB);
	window_manager->g_buffer.render();
	glDisable(GL_FRAMEBUFFER_SRGB);
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);

	glDepthMask(GL_FALSE);
	root.get_child("Foreground").render();
	root.get_child("Chara Slots").render();
	root.get_child("Player Cursors").render();
}

/// <summary>
/// Select the CSS slot that the cursor is hovering over. Works by comparing the column and row of each slot to the column and row of the player, so 
/// make sure to set that up properly if this function is copied into another menu.
/// </summary>
void CSS::select_slot(int player_idx) {
	SceneElement& cursor = root.get_child("Player Cursors").get_child(player_idx);
	SceneElement& chara_slot = root.get_child("Chara Slots").get_child(cursor.int_var("selected_slot"));

	cursor.get_screen_texture("Cursor").set_pos(chara_slot.get_screen_texture("Chara Render").get_pos_target().get_target_val(), 8);

	cursor.int_var("selected_costume") = player[player_idx]->player_info->preferred_costume[chara_slot.int_var("chara_kind")];
	cursor.int_var("selected_color") = player[player_idx]->player_info->preferred_color[chara_slot.int_var("chara_kind")];

	ScreenTexture& large_css_slot = cursor.get_screen_texture("Chara Slot Large");

	large_css_slot = ScreenTexture(chara_slot.get_screen_texture("Chara Render"))
		.set_scale(1.25f)
		.set_pos(glm::vec3(player_id ? 3709.5f : 103.5f, 418.0f, 0))
		.set_orientation(TEXTURE_BOTTOM_LEFT)
		.set_alpha(127);

	if (demo_models[player_idx].model.is_loaded()) {
		demo_models[player_idx].model.unload_model_instance();
	}
	if (cursor.int_var("selected_slot") < loaded_chars && cursor.int_var("selected_slot") != -1) {
		demo_models[player_idx].load_model(
			"resource/chara/" + chara_slot.string_var("resource_name") +
			"/model/m" + std::to_string(cursor.int_var("selected_costume")) + "/model.fbx",
			"c" + std::to_string(cursor.int_var("selected_color"))
		);
		demo_models[player_idx].init_shader();
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
	SceneElement& player_cursor = root.get_child("Player Cursors").get_child(player_idx);
	SceneElement& selected_slot = root.get_child("Chara Slots").get_child(player_cursor.int_var("selected_slot"));
	demo_models[player_idx].load_model(
		"resource/chara/" + selected_slot.string_var("resource_name") +
		"/model/m" + std::to_string(player_cursor.int_var("selected_costume")) + "/model.fbx",
		"c0"
	);
}

void CSS::select_color(int player_idx) {
	if (demo_models[player_idx].model.is_loaded()) {
		demo_models[player_idx].model.unload_textures();
		demo_models[player_idx].model.load_textures("c" +
			std::to_string(root.get_child("Player Cursors").get_child(player_idx).int_var("selected_color"))
		);
	}
}

/// <summary>
/// Takes the character that the current player is already selecting, and automatically puts their cursor on the first CSS slot corresponding to that
/// character. Note: This function is used for when you re-enter the CSS after having already selected a character, such as after a match.
/// </summary>
/// <param name="chara_kind">: The character whose CSS slot to select.</param>
void CSS::select_preferred_chara_kind(PlayerInfo* player_info) {
	int preferred_chara = player_info->preferred_chara;
	int preferred_costume = player_info->preferred_costume[preferred_chara];
	int preferred_color = player_info->preferred_color[preferred_chara];
	if (player_info == SaveManager::get_instance()->get_player_info(-1)) {
		preferred_chara = player[player_id]->chara_kind;
		preferred_costume = player[player_id]->alt_costume;
		preferred_color = player[player_id]->alt_color;
	}
	SceneElement& player_cursor = root.get_child("Player Cursors").get_child(player_id);
	for (int i = 0, max = root.get_child("Chara Slots").int_var("num_slots"); i < max; i++) {
		if (root.get_child("Chara Slots").get_child(i).int_var("chara_kind") == preferred_chara) {
			player_cursor.int_var("selected_slot") = i;
			break;
		}
	}
	player_cursor.int_var("selected_costume") = preferred_costume;
	player_cursor.int_var("selected_color") = preferred_color;
}

void CSS::load_chara_model_into_main_thread() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	resource_manager->init_gl_model("resource/chara/"
		+ root.get_child("Chara Slots").get_child(loaded_chars).string_var("resource_name")
		+ "/model/m0/model.fbx"
	);

	for (int i = 0; i < 2; i++) {
		SceneElement& cursor = root.get_child("Player Cursors").get_child(i);
		int selection = cursor.int_var("selected_slot");
		if (loaded_chars == selection) {
			if (demo_models[i].model.is_loaded()) {
				demo_models[i].model.unload_model_instance();
			}
			demo_models[i].load_model(
				"resource/chara/"
				+ root.get_child("Chara Slots").get_child(selection).string_var("resource_name")
				+ "/model/m" + std::to_string(cursor.int_var("selected_costume")) + "/model.fbx",
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