#include "PauseBattle.h"
#include "WindowManager.h"
#include "SaveManager.h"
#include "ObjectManager.h"
#include "Fighter.h"
#include "TimeFuncs.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	Scene* background_menu = game_manager->get_scene();

	PauseBattle* pause = new PauseBattle;

	while (pause->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();

		pause->process();
		pause->render();
		
		window_manager->update_screen();
	}

	delete pause;
}

PauseBattle::PauseBattle() {
	GameManager* game_manager = GameManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	int text_height_diff = -120;
	load_font("header", "FiraCode", 96);
	load_font("button", "FiraCode", 64);

	if (context == SCENE_CONTEXT_TRAINING) {
		load_event("Page Activate", [this](SceneElement* e) {
			SaveManager::get_instance()->set_game_setting("pause_battle_training_page", e->get_id());
			set_active_element(&e->get_child(e->int_var("selection")));
		});
		load_event("Page Page Left Press", [this](SceneElement* e) {
			e->hide();
			SceneElement* s = &e->get_prev_sibling();
			set_active_element(s);
			s->show();
		});
		load_event("Page Page Right Press", [this](SceneElement* e) {
			e->hide();
			SceneElement* s = &e->get_next_sibling();
			set_active_element(s);
			s->show();
		});
	}
	load_event("Button Up Press", [this](SceneElement* e) {
		for (int i = 0; i < e->get_num_screen_texts(); i++) {
			e->get_screen_text(i).set_scale(1.0f);
		}
		SceneElement* s = &e->get_prev_sibling_bounded();
		while (!s->is_visible()) {
			if (s == &s->get_prev_sibling_bounded()) {
				s = e;
				break;
			}
			s = &s->get_prev_sibling_bounded();
		}
		for (int i = 0; i < s->get_num_screen_texts(); i++) {
			s->get_screen_text(i).set_scale(1.1f);
		}
		s->get_parent().int_var("selection") = s->get_id();
		set_active_element(s);
	});
	load_event("Button Down Press", [this](SceneElement* e) {
		for (int i = 0; i < e->get_num_screen_texts(); i++) {
			e->get_screen_text(i).set_scale(1.0f);
		}
		SceneElement* s = &e->get_next_sibling_bounded();
		while (!s->is_visible()) {
			if (s == &s->get_next_sibling_bounded()) {
				s = e;
				break;
			}
			s = &s->get_next_sibling_bounded();
		}
		for (int i = 0; i < s->get_num_screen_texts(); i++) {
			s->get_screen_text(i).set_scale(1.1f);
		}
		s->get_parent().int_var("selection") = s->get_id();
		set_active_element(s);
	});
	load_event("Button Page Left Press", [this](SceneElement* e) {
		e->get_parent().execute_event("page_left_press");
	});
	load_event("Button Page Left Press", [this](SceneElement* e) {
		e->get_parent().execute_event("page_right_press");
	});
	load_event("Button Create Confirm Window", [this](SceneElement* e) {
		SceneElement* confirm = &get_element("root/Confirm Window");
		confirm->ptr_var("element_to_confirm", e);
		set_active_element(confirm);
	});
	load_event("Start Press", [this](SceneElement* e) {
		looping = false;
	});

	root.add_elements({
		{ "BG Dimmer", ScreenTexture("resource/misc/fade.png").set_alpha(127) },
		{ "Panel", ScreenTexture("resource/scene/battle/ui/pause/overlay.png") },
		{ "Pages", SceneElement({
			{ "Main Page",
				SceneElement({
					{"Label", ScreenText(&get_font("header"), "Pause", TextSpecifier().border(2))
						.set_orientation(TEXTURE_TOP)
					},
					{"Character Select",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Return to Character Select", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP)
								.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
								.set_scale(1.1f)
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("select_press", get_event("Button Create Confirm Window"))
						.add_event("select_confirmed", [this](SceneElement* e) {
							GameManager* game_manager = GameManager::get_instance();
							game_manager->update_scene(SCENE_CHARA_SELECT);
							game_manager->get_scene(1)->looping = false;
						})
						.add_event("start_press", get_event("Start Press"))
					},
					{"Stage Select",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Return to Stage Select", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP)
								.set_pos(glm::vec3(0.0f, 320.0f, 0.0f))
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("select_press", get_event("Button Create Confirm Window"))
						.add_event("select_confirmed", [this](SceneElement* e) {
							GameManager* game_manager = GameManager::get_instance();
							game_manager->update_scene(SCENE_STAGE_SELECT);
							game_manager->get_scene(1)->looping = false;
						})
						.add_event("start_press", get_event("Start Press"))
					},
					{"Main Menu",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Return to Main Menu", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP)
								.set_pos(glm::vec3(0.0f, 440.0f, 0.0f))
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("select_press", get_event("Button Create Confirm Window"))
						.add_event("select_confirmed", [this](SceneElement* e) {
							GameManager* game_manager = GameManager::get_instance();
							game_manager->update_scene(SCENE_MAIN_MENU);
							game_manager->get_scene(1)->looping = false;
						})
						.add_event("start_press", get_event("Start Press"))
					},
					{"Options",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Options", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP)
								.set_pos(glm::vec3(0.0f, 560.0f, 0.0f))
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("select_press", [this](SceneElement* e) {
							//TODO: Options Menu
						})
						.add_event("start_press", get_event("Start Press"))
					},
				})
				.int_var("selection", 0)
				.add_event("activate", get_event("Page Activate", true))
				.add_event("page_left_press", get_event("Page Page Left Press", true))
				.add_event("page_right_press", get_event("Page Page Right Press", true))
			},
			{ "Training Page",
				SceneElement({
					{"Label", ScreenText(&get_font("header"), "Training Settings", TextSpecifier().border(2))
						.set_orientation(TEXTURE_TOP)
					},
					{"Hitbox Visuals",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Hitbox Visuals", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP_LEFT)
								.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
								.set_scale(1.1f)
							},
							{"Setting", ScreenText(&get_font("button"), "Off", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP_RIGHT)
								.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
								.set_scale(1.1f)
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("left_press", [this](SceneElement* e) {
							SaveManager* save_manager = SaveManager::get_instance();
							if (save_manager->get_game_setting("visualize_boxes")) {
								e->get_screen_text("Setting").update_text("Off");
								save_manager->set_game_setting("visualize_boxes", 0);
							}
							else {
								e->get_screen_text("Setting").update_text("On");
								save_manager->set_game_setting("visualize_boxes", 1);
							}
						})
						.add_event("right_press", [this](SceneElement* e) {
							e->execute_event("left_press");
						})
						.add_event("start_press", get_event("Start Press"))
					},
					{"Reset",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "Reset", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP)
								.set_pos(glm::vec3(0.0f, 320.0f, 0.0f))
							}
						})
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("select_press", [this](SceneElement* e) {
							ObjectManager* object_manager = ObjectManager::get_instance();
							WindowManager* window_manager = WindowManager::get_instance();
							object_manager->fighter[0]->reset();
							object_manager->fighter[1]->reset();
							window_manager->camera.reset_camera();
							this->looping = false;
						})
						.add_event("start_press", get_event("Start Press"))
					}
				})
				.int_var("selection", 0)
				.add_event("activate", get_event("Page Activate", true))
				.add_event("page_left_press", get_event("Page Page Left Press", true))
				.add_event("page_right_press", get_event("Page Page Right Press", true))
			},
			{ "CPU Page",
				SceneElement({
					{"Label", ScreenText(&get_font("header"), "CPU Settings", TextSpecifier().border(2))
						.set_orientation(TEXTURE_TOP)
					},
					{"CPU Mode",
						SceneElement({
							{"Label", ScreenText(&get_font("button"), "P2 Mode", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP_LEFT)
								.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
								.set_scale(1.1f)
							},
							{"Setting", ScreenText(&get_font("button"), "Player Controlled", TextSpecifier().border(2))
								.set_orientation(TEXTURE_TOP_RIGHT)
								.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
								.set_scale(1.1f)
							},
						})
						.int_var("p2_mode", 0)
						.add_event("up_press", get_event("Button Up Press"))
						.add_event("down_press", get_event("Button Down Press"))
						.add_event("hide_current_p2_mode", [this](SceneElement* e) {
							switch (e->int_var("p2_mode")) {
								case 0: {

								} break;
								case 1: {

								} break;
								case 2: {

								} break;
							}
						})
						.add_event("show_current_p2_mode", [this](SceneElement* e) {
							switch (e->int_var("p2_mode")) {
								case 0: {
									e->get_screen_text("Setting").update_text("Player Controlled");
								} break;
								case 1: {
									e->get_screen_text("Setting").update_text("CPU Controlled");
								} break;
								case 2: {
									e->get_screen_text("Setting").update_text("Dummy");
								} break;
							}
						})
						.add_event("left_press", [this](SceneElement* e) {
							e->execute_event("hide_current_p2_mode");
							int& p2_mode = e->int_var("p2_mode");
							p2_mode = (p2_mode - 1) % 3;
							e->execute_event("show_current_p2_mode");
						})
						.add_event("right_press", [this](SceneElement* e) {
							e->execute_event("hide_current_p2_mode");
							int& p2_mode = e->int_var("p2_mode");
							p2_mode = (p2_mode + 1) % 3;
							e->execute_event("show_current_p2_mode");
						})
						.add_event("page_left_press", get_event("Button Page Left Press"))
						.add_event("page_right_press", get_event("Button Page Right Press"))
						.add_event("start_press", get_event("Start Press"))
					}
				})
				.int_var("selection", 0)
				.add_event("activate", get_event("Page Activate", true))
				.add_event("page_left_press", get_event("Page Page Left Press", true))
				.add_event("page_right_press", get_event("Page Page Right Press", true))
			}
		})},
		{ "Confirm Window",
			SceneElement({
				{"Panel", ScreenTexture("resource/scene/battle/ui/pause/confirm.png") },
				{"Label", ScreenText(&get_font("header"), "Confirm?", TextSpecifier().border(2))
					.set_pos(glm::vec3(0.0f, 90.0f, 0.0f))
				},
				{"Yes", ScreenText(&get_font("button"), "Yes", TextSpecifier().border(2)) 
					.set_pos(glm::vec3(200.0f, 0.0f, 0.0f))
					.set_scale(1.1f)
				},
				{"No", ScreenText(&get_font("button"), "No", TextSpecifier().border(2)) 
					.set_pos(glm::vec3(-200.0f, 0.0f, 0.0f))
				},
			})
			.bool_var("yes", true)
			.ptr_var("element_to_confirm", nullptr)
			.add_event("left_press", [this](SceneElement* e) {
				e->bool_var("yes") = true;
				e->get_screen_text("Yes").set_scale(1.1f);
				e->get_screen_text("No").set_scale(1.0f);
			})
			.add_event("right_press", [this](SceneElement* e) {
				e->bool_var("yes") = false;
				e->get_screen_text("No").set_scale(1.1f);
				e->get_screen_text("Yes").set_scale(1.0f);
			})
			.add_event("activate", [this](SceneElement* e) {
				e->show();
			})
			.add_event("select_press", [this](SceneElement* e) {
				if (e->bool_var("yes")) {
					((SceneElement*)e->ptr_var("element_to_confirm"))->execute_event("select_confirmed");
				}
				else {
					set_active_element((SceneElement*)e->ptr_var("element_to_confirm"));
					e->hide();
				}			
			})
			.add_event("back_press", [this](SceneElement* e) {
				set_active_element((SceneElement*)e->ptr_var("element_to_confirm"));
				e->hide();
			})
		}
	}).set_anchor_dimensions(
		get_screen_texture("root/Panel").get_base_width(), 
		get_screen_texture("root/Panel").get_base_height()
	);
}


PauseBattle::~PauseBattle() {

}

void PauseBattle::process_main() {

}

void PauseBattle::render_main() {
	root.render();
}