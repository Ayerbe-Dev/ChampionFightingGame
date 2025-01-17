#pragma warning(disable : 4996)
#include "MainMenu.h"
#include "GameManager.h"
#include <glew/glew.h>
#include "GLM Helpers.h"
#include "WindowConstants.h"
#include "Player.h"
#include <math.h>
#include "cotr_imgui_debugger.h"
#include "DebugMenu.h"
#include "Options.h"
#include "WindowManager.h"
#include "FontManager.h"
#include "SoundManager.h"

void main_menu_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	
	font_manager->load_face("Fiend-Oblique");

	MainMenu *main_menu = new MainMenu;

	while (main_menu->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();

		main_menu->process();
		main_menu->render();

		window_manager->update_screen();

		if (main_menu->sub_scene != SCENE_NONE) {
			if (game_manager->scene_main[main_menu->sub_scene] != nullptr) {
				game_manager->scene_main[main_menu->sub_scene]();
			}
			else {
				game_manager->add_crash_log("Error: Game State was " + std::to_string(main_menu->sub_scene) + " (not GAME_SUBSTATE_NONE) but there was no associated function!");
				game_manager->update_scene(SCENE_DEBUG_MENU);
			}
			main_menu->sub_scene = SCENE_NONE;
		}
	}

	font_manager->unload_face("Fiend-Oblique");
	delete main_menu;
}

MainMenu::MainMenu() {
	offset = 12.5;
	magnitude = 1280;
	menu_frame = 0;

	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	FontManager* font_manager = FontManager::get_instance();
	load_font("main_text", "Fiend-Oblique", 144);
	load_font("sub_text", "Fiend-Oblique", 80);
	TextSpecifier main_spec = TextSpecifier().color(glm::vec4(255.0, 127.0, 0.0, 255.0)).border(glm::vec4(1.0, 1.0, 1.0, 2.0));
	TextSpecifier sub_spec = TextSpecifier().color(glm::vec4(255.0, 127.0, 0.0, 255.0)).border(1);

	load_event("Top Level Up EX", [this](SceneElement* e) {
		get_element("root/Rotating Text").int_var("top_selection") = 4;
		get_element("root/Rotating Text").add_rot(glm::vec3(0.0f, 0.0f, offset * 5));
		e->hide();
		set_active_element(&e->get_sibling("Top Menu Extras"));
	});
	load_event("Top Level Up", [this](SceneElement* e) {
		get_element("root/Rotating Text").int_var("top_selection")--;
		e->hide();
		set_active_element(&e->get_prev_sibling());
	});
	load_event("Top Level Down EX", [this](SceneElement* e) {
		get_element("root/Rotating Text").int_var("top_selection") = 0;
		get_element("root/Rotating Text").add_rot(glm::vec3(0.0f, 0.0f, -offset * 5));
		e->hide();
		set_active_element(&e->get_sibling("Top Menu Online"));
	});
	load_event("Top Level Down", [this](SceneElement* e) {
		get_element("root/Rotating Text").int_var("top_selection")++;
		e->hide();
		set_active_element(&e->get_next_sibling());
	});
	load_event("Top Level Select", [this](SceneElement* e) {
		set_active_element(&e->get_child("Sub Menu"));
	});
	load_event("Top Level Back", [this](SceneElement* e) {
		update_scene(SCENE_TITLE_SCREEN, SCENE_CONTEXT_NONE);
	});
	load_event("Top Level Activate", [this](SceneElement* e) {
		e->show();
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc"));
	});
	load_event("Sub Level Up", [this](SceneElement* e) {
		int& selection = e->int_var("selection");
		if (selection != 0) {
			selection--;
		}
		e->get_screen_texture("Cursor").set_pos(glm::vec3(
			0.0f,
			e->get_screen_text(selection).get_pos().y,
			0.0f
		));
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc" + std::to_string(selection)));
	});
	load_event("Sub Level Down", [this](SceneElement* e) {
		int& selection = e->int_var("selection");
		if (selection != e->get_num_screen_texts() - 1) {
			selection++;
		}
		e->get_screen_texture("Cursor").set_pos(glm::vec3(
			0.0f,
			e->get_screen_text(selection).get_pos().y,
			0.0f
		));
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc" + std::to_string(selection)));
	});
	load_event("Sub Level Back", [this](SceneElement* e) {
		set_active_element(&e->get_parent());
	});
	load_event("Sub Level Activate", [this](SceneElement* e) {
		e->set_pos(glm::vec3(1805.0f, 0.0f, 0.0f), 5);
		int& selection = e->int_var("selection");
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc" + std::to_string(selection)));
		menu_frame = 1;
	});
	load_event("Sub Level Deactivate", [this](SceneElement* e) {
		e->set_pos(glm::vec3(4065.0f, 0.0f, 0.0f), 5); //TODO: I'm pretty sure this visually breaks if
		//you go from the sub level to top level and then switch top menu options within 5 frames since
		//it will hide the sub menu
		menu_frame = 0;
	});

	root.add_elements({
		{"Background",
			SceneElement({
				{"bg", ScreenTexture("resource/scene/menu/main/bg.gif") }}
			)
			.add_event("on_render", [this](SceneElement* e) {
				e->get_screen_texture("bg").set_sprite(menu_frame);
			})
		},
		{"Top Menu Online",
			SceneElement({
				{"BG Online", ScreenTexture("resource/scene/menu/main/missingno.png")
					.set_orientation(TEXTURE_LEFT)
				},
				{"Sub Menu",
					SceneElement({
						{"Sub Table", ScreenTexture("resource/scene/menu/main/SubMenu.png")},
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Lobby Text", ScreenText(&get_font("sub_text"), "Lobby", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Queue Text", ScreenText(&get_font("sub_text"), "Queue", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Coach Text", ScreenText(&get_font("sub_text"), "Coach", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						}
					})
					.add_event("up_press", get_event("Sub Level Up"))
					.add_event("down_press", get_event("Sub Level Down"))
					.add_event("select_press", [this](SceneElement* e) {
						switch (e->int_var("selection")) {
							case 0: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 1: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 2: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							default: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							}
						}
					})
					.add_event("left_press", get_event("Sub Level Back"))
					.add_event("back_press", get_event("Sub Level Back"))
					.add_event("activate", get_event("Sub Level Activate"))
					.add_event("deactivate", get_event("Sub Level Deactivate"))
					.set_anchor_dimensions(450, 640)
					.int_var("selection", 0)
					.set_pos(glm::vec3(4065.0f, 0.0f, 0.0f))
				},
			})
			.add_event("up_press", get_event("Top Level Up EX"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{"Top Menu Solo", 
			SceneElement({
				{"BG Solo", ScreenTexture("resource/scene/menu/main/missingno.png")
					.set_orientation(TEXTURE_LEFT)
				},
				{"Sub Menu",
					SceneElement({
						{"Sub Table", ScreenTexture("resource/scene/menu/main/SubMenu.png")},
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")},
						{"Story Text", ScreenText(&get_font("sub_text"), "Story", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Arcade Text", ScreenText(&get_font("sub_text"), "Arcade", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Training Text", ScreenText(&get_font("sub_text"), "Training", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						}
					})
					.add_event("up_press", get_event("Sub Level Up"))
					.add_event("down_press", get_event("Sub Level Down"))
					.add_event("select_press", [this](SceneElement* e) {
						switch (e->int_var("selection")) {
							case 0: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 1: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 2: {
								update_scene(SCENE_STAGE_SELECT, SCENE_CONTEXT_TRAINING);
							} break;
							default: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							}
						}
					})
					.add_event("left_press", get_event("Sub Level Back"))
					.add_event("back_press", get_event("Sub Level Back"))
					.add_event("activate", get_event("Sub Level Activate"))
					.add_event("deactivate", get_event("Sub Level Deactivate"))
					.set_anchor_dimensions(450, 640)
					.int_var("selection", 0)
					.set_pos(glm::vec3(4065.0f, 0.0f, 0.0f))
				},
			})
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{"Top Menu VS", 
			SceneElement({
				{"BG VS", ScreenTexture("resource/scene/menu/main/vsimg.png")
					.set_orientation(TEXTURE_LEFT)
				},
				{"Sub Menu",
					SceneElement({
						{"Sub Table", ScreenTexture("resource/scene/menu/main/SubMenu.png")},
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Battle Text", ScreenText(&get_font("sub_text"), "Battle", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Tournament Text", ScreenText(&get_font("sub_text"), "Tournament", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Special Text", ScreenText(&get_font("sub_text"), "Special", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						}
					})
					.add_event("up_press", get_event("Sub Level Up"))
					.add_event("down_press", get_event("Sub Level Down"))
					.add_event("select_press", [this](SceneElement* e) {
						switch (e->int_var("selection")) {
							case 0: {
								update_scene(SCENE_STAGE_SELECT, SCENE_CONTEXT_NONE);
							} break;
							case 1: {
								update_scene(SCENE_STAGE_SELECT, SCENE_CONTEXT_NONE);
							} break;
							case 2: {
								update_scene(SCENE_STAGE_SELECT, SCENE_CONTEXT_SPECIAL);
							} break;
							default: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							}
						}
					})
					.add_event("left_press", get_event("Sub Level Back"))
					.add_event("back_press", get_event("Sub Level Back"))
					.add_event("activate", get_event("Sub Level Activate"))
					.add_event("deactivate", get_event("Sub Level Deactivate"))
					.set_anchor_dimensions(450, 640)
					.int_var("selection", 0)
					.set_pos(glm::vec3(4065.0f, 0.0f, 0.0f))
				},
			})
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{"Top Menu Options", 
			SceneElement({
				{"BG Options", ScreenTexture("resource/scene/menu/main/missingno.png")
					.set_orientation(TEXTURE_LEFT)
				},
				{"Sub Menu",
					SceneElement({
						{"Sub Table", ScreenTexture("resource/scene/menu/main/SubMenu.png")},
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Controls Text", ScreenText(&get_font("sub_text"), "Controls", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Graphics Text", ScreenText(&get_font("sub_text"), "Graphics", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Sound Text", ScreenText(&get_font("sub_text"), "Sound", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Save Data Text", ScreenText(&get_font("sub_text"), "Save Data", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Placeholder Text", ScreenText(&get_font("sub_text"), "Placeholder", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						}
					})
					.add_event("up_press", get_event("Sub Level Up"))
					.add_event("down_press", get_event("Sub Level Down"))
					.add_event("select_press", [this](SceneElement* e) {
						switch (e->int_var("selection")) {
							case 0: {
								sub_scene = SCENE_CONTROLS;
							} break;
							case 1: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 2: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 3: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 4: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							default: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							}
						}
					})
					.add_event("left_press", get_event("Sub Level Back"))
					.add_event("back_press", get_event("Sub Level Back"))
					.add_event("activate", get_event("Sub Level Activate"))
					.add_event("deactivate", get_event("Sub Level Deactivate"))
					.set_anchor_dimensions(450, 640)
					.int_var("selection", 0)
					.set_pos(glm::vec3(4065.0f, 0.0f, 0.0f))
				},
			})
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{"Top Menu Extras", 
			SceneElement({
				{"BG Extras", ScreenTexture("resource/scene/menu/main/missingno.png")
					.set_orientation(TEXTURE_LEFT)
				},
				{"Sub Menu",
					SceneElement({
						{"Sub Table", ScreenTexture("resource/scene/menu/main/SubMenu.png")},
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Gallery Text", ScreenText(&get_font("sub_text"), "Gallery", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Sound Test Text", ScreenText(&get_font("sub_text"), "Sound Test", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						},
						{"Cutscenes Text", ScreenText(&get_font("sub_text"), "Cutscenes", sub_spec)
							.set_screen_orientation(TEXTURE_TOP_LEFT)
						}
					})
					.add_event("up_press", get_event("Sub Level Up"))
					.add_event("down_press", get_event("Sub Level Down"))
					.add_event("select_press", [this](SceneElement* e) {
						switch (e->int_var("selection")) {
							case 0: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 1: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							case 2: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							} break;
							default: {
								update_scene(SCENE_DEBUG_MENU, SCENE_CONTEXT_NONE);
							}
						}
					})
					.add_event("left_press", get_event("Sub Level Back"))
					.add_event("back_press", get_event("Sub Level Back"))
					.add_event("activate", get_event("Sub Level Activate"))
					.add_event("deactivate", get_event("Sub Level Deactivate"))
					.set_anchor_dimensions(450, 640)
					.int_var("selection", 0)
					.set_pos(glm::vec3(4065.0f, 0.0f, 0.0f))
				},
			})
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down EX"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{ "Rotating Text",
			SceneElement({
				//TODO: These all need to be given their starting pos/rot values
				{"Online Label", ScreenText(&get_font("main_text"), "Online", main_spec)
					.set_pos(get_circular_pos(glm::vec3(-114.0f, 0.0f, 0.0f), magnitude, 2 * glm::radians(offset)))
					.set_rot(glm::vec3(0.0, 0.0, 2 * offset))
				},
				{"Solo Label", ScreenText(&get_font("main_text"), "Solo", main_spec)
					.set_pos(get_circular_pos(glm::vec3(-114.0f, 0.0f, 0.0f), magnitude, glm::radians(offset)))
					.set_rot(glm::vec3(0.0, 0.0, offset))
				},
				{"VS Label", ScreenText(&get_font("main_text"), "VS", main_spec)
					.set_pos(get_circular_pos(glm::vec3(-114.0f, 0.0f, 0.0f), magnitude, 0))
				},
				{"Options Label", ScreenText(&get_font("main_text"), "Options", main_spec)
					.set_pos(get_circular_pos(glm::vec3(-114.0f, 0.0f, 0.0f), magnitude, -glm::radians(offset)))
					.set_rot(glm::vec3(0.0, 0.0, -offset))
				},
				{"Extras Label", ScreenText(&get_font("main_text"), "Extras", main_spec)
					.set_pos(get_circular_pos(glm::vec3(-114.0f, 0.0f, 0.0f), magnitude, -2 * glm::radians(offset)))
					.set_rot(glm::vec3(0.0, 0.0, -2 * offset))
				},
			})
			.add_event("on_render", [this](SceneElement* e) {
				float rot_z = e->get_rot().z;
				e->add_rot(glm::vec3(0.0f, 0.0f, (((e->int_var("top_selection") - 2) * offset) - e->get_rot().z) / 8));

//				e->add_rot(glm::vec3(0.0f, 0.0f, offset * 5));
//				e->render();
//				e->add_rot(glm::vec3(0.0f, 0.0f, offset * -10));
//				e->render();
//				e->add_rot(glm::vec3(0.0f, 0.0f, offset * 5));
			})
			.int_var("top_selection", 2)
//			.set_pos(glm::vec3(-114, 0.0f, 0.0f))
		},
		{"Descriptions",
			SceneElement({
				{"Desc Bar", ScreenTexture("resource/scene/menu/main/DescBar.png")
					.set_orientation(TEXTURE_BOTTOM)
				},
				{"Desc Text", ScreenText(&get_font("sub_text"), "Placeholder", sub_spec)
					.set_orientation(TEXTURE_BOTTOM)
				},
			})
		}
	});
	for (int i = 1; i < 5; i++) {
		SceneElement& e = root.get_child(i).get_child("Sub Menu");
		for (int j = 0; j < e.get_num_screen_texts(); j++) {
			e.get_screen_text(j).add_pos(glm::vec3(0.0f, 60.0f * j, 0.0f));
		}
	}
	set_active_element(&get_element("root/Top Menu VS"));
}

MainMenu::~MainMenu() {

}

void MainMenu::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	std::cout << get_active_element().get_full_name() << "\n";
}

void MainMenu::render_main() {
	root.render();
}