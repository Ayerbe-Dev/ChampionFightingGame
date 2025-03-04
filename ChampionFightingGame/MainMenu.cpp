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
	menu_frame = 0;

	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	FontManager* font_manager = FontManager::get_instance();
	load_font("main_text", "Fiend-Oblique", 96);
	load_font("sub_text", "Fiend-Oblique", 64);
	load_font("desc_text", "FiraCode", 48);
	TextSpecifier main_spec = TextSpecifier().color(glm::vec4(255.0, 127.0, 0.0, 255.0)).border(glm::vec4(1.0, 1.0, 1.0, 2.0));
	TextSpecifier sub_spec = TextSpecifier().color(glm::vec4(255.0, 127.0, 0.0, 255.0)).border(1);
	TextSpecifier desc_spec = TextSpecifier().border(1);

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
			100.0f,
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
			100.0f,
			e->get_screen_text(selection).get_pos().y,
			0.0f
		));
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc" + std::to_string(selection)));
	});
	load_event("Sub Level Back", [this](SceneElement* e) {
		set_active_element(&e->get_parent());
	});
	load_event("Sub Level Activate", [this](SceneElement* e) {
		e->set_pos(glm::vec3(-950.0f, 0.0f, 0.0f), 5);
		int& selection = e->int_var("selection");
		get_screen_text("root/Descriptions/Desc Text").update_text(e->string_var("Desc" + std::to_string(selection)));
		menu_frame = 1;
	});
	load_event("Sub Level Deactivate", [this](SceneElement* e) {
		e->set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f), 5); 
		//TODO: I'm pretty sure this visually breaks if you go from the sub level to top level and 
		//then switch top menu options within 5 frames since it will hide the sub menu before it has a
		//chance to go off screen

		//It probably isn't super noticeable, but hypothetically this could be fixed by setting all 5
		//sub menu elements to go off screen rather than just the specific one we're closing
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
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")},
						{"Lobby Text", ScreenText(&get_font("sub_text"), "Lobby", sub_spec)},
						{"Queue Text", ScreenText(&get_font("sub_text"), "Queue", sub_spec)},
						{"Coach Text", ScreenText(&get_font("sub_text"), "Coach", sub_spec)}
					})
					.int_var("selection", 0)
					.string_var("Desc0", "Join an online lobby")
					.string_var("Desc1", "Queue for a match")
					.string_var("Desc2", "Spectate and coach a random player")
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
					.set_orientation(TEXTURE_RIGHT)
					.set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f))
				},
			})
			.string_var("Desc", "Take your gameplay online!")
			.add_event("up_press", get_event("Top Level Up EX"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("right_press", get_event("Top Level Select"))
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
						{"Story Text", ScreenText(&get_font("sub_text"), "Story", sub_spec)},
						{"Arcade Text", ScreenText(&get_font("sub_text"), "Arcade", sub_spec)},
						{"Training Text", ScreenText(&get_font("sub_text"), "Training", sub_spec)}
					})
					.int_var("selection", 0)
					.string_var("Desc0", "Play through the story!")
					.string_var("Desc1", "It's arcade mode bro idk")
					.string_var("Desc2", "hit the lab idiot")
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
					.set_orientation(TEXTURE_RIGHT)
					.set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f))
				},
			})
			.string_var("Desc", "Single player content!")
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("right_press", get_event("Top Level Select"))
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
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")},
						{"Battle Text", ScreenText(&get_font("sub_text"), "Battle", sub_spec)},
						{"Tournament Text", ScreenText(&get_font("sub_text"), "Tournament", sub_spec)},
						{"Special Text", ScreenText(&get_font("sub_text"), "Special", sub_spec)}
					})
					.int_var("selection", 0)
					.string_var("Desc0", "Kill your friends, guilt free")
					.string_var("Desc1", "is this bracket")
					.string_var("Desc2", "Mix it up with unique scenarios")
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
					.set_orientation(TEXTURE_RIGHT)
					.set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f))
				},
			})
			.string_var("Desc", "This do be the VS mode")
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("right_press", get_event("Top Level Select"))
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
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")},
						{"Controls Text", ScreenText(&get_font("sub_text"), "Controls", sub_spec)},
						{"Graphics Text", ScreenText(&get_font("sub_text"), "Graphics", sub_spec)},
						{"Sound Text", ScreenText(&get_font("sub_text"), "Sound", sub_spec)},
						{"Save Data Text", ScreenText(&get_font("sub_text"), "Save Data", sub_spec)},
						{"Placeholder Text", ScreenText(&get_font("sub_text"), "Placeholder", sub_spec)}
					})
					.int_var("selection", 0)
					.string_var("Desc0", "Control settings")
					.string_var("Desc1", "Graphics settings")
					.string_var("Desc2", "Sound settngs")
					.string_var("Desc3", "Save data settings")
					.string_var("Desc4", "idk i'll probably add a 5th options tab")
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
					.set_orientation(TEXTURE_RIGHT)
					.set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f))
				},
			})
			.string_var("Desc", "Bro it's an options menu")
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down"))
			.add_event("right_press", get_event("Top Level Select"))
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
						{"Cursor", ScreenTexture("resource/scene/menu/main/Cursor.png")},
						{"Gallery Text", ScreenText(&get_font("sub_text"), "Gallery", sub_spec)},
						{"Sound Test Text", ScreenText(&get_font("sub_text"), "Sound Test", sub_spec)},
						{"Cutscenes Text", ScreenText(&get_font("sub_text"), "Cutscenes", sub_spec)}
					})
					.int_var("selection", 0)
					.string_var("Desc0", "Check out various art")
					.string_var("Desc1", "Listen to music and voice lines")
					.string_var("Desc2", "Watch the cutscenes from the story mode")
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
					.set_orientation(TEXTURE_RIGHT)
					.set_pos(glm::vec3(-1900.0f, 0.0f, 0.0f))
				},
			})
			.string_var("Desc", "Check out the extra content that the game has to offer!")
			.add_event("up_press", get_event("Top Level Up"))
			.add_event("down_press", get_event("Top Level Down EX"))
			.add_event("right_press", get_event("Top Level Select"))
			.add_event("select_press", get_event("Top Level Select"))
			.add_event("back_press", get_event("Top Level Back"))
			.add_event("activate", get_event("Top Level Activate"))
			.hide()
		},
		{ "Rotating Text",
			SceneElement({
				//TODO: These all need to be given their starting pos/rot values
				{"Online Label", ScreenText(&get_font("main_text"), "Online", main_spec)
					.set_magnitude(glm::vec3(4200.0f, 0.0f, 0.0f))
					.set_rot(glm::vec3(0.0, 0.0, 2 * offset))
				},
				{"Solo Label", ScreenText(&get_font("main_text"), "Solo", main_spec)
					.set_magnitude(glm::vec3(4200.0f, 0.0f, 0.0f))
					.set_rot(glm::vec3(0.0, 0.0, offset))
				},
				{"VS Label", ScreenText(&get_font("main_text"), "VS", main_spec)
					.set_magnitude(glm::vec3(4200.0f, 0.0f, 0.0f))
				},
				{"Options Label", ScreenText(&get_font("main_text"), "Options", main_spec)
					.set_magnitude(glm::vec3(4200.0f, 0.0f, 0.0f))
					.set_rot(glm::vec3(0.0, 0.0, -offset))
				},
				{"Extras Label", ScreenText(&get_font("main_text"), "Extras", main_spec)
					.set_magnitude(glm::vec3(4200.0f, 0.0f, 0.0f))
					.set_rot(glm::vec3(0.0, 0.0, -2 * offset))
				},
			})
			.add_event("on_render", [this](SceneElement* e) {
				float rot_z = e->get_rot().z;
				e->add_rot(glm::vec3(0.0f, 0.0f, (((e->int_var("top_selection") - 2) * offset) - e->get_rot().z) / 8));

				e->add_rot(glm::vec3(0.0f, 0.0f, offset * 5));
				e->render();
				e->add_rot(glm::vec3(0.0f, 0.0f, offset * -10));
				e->render();
				e->add_rot(glm::vec3(0.0f, 0.0f, offset * 5));
			})
			.set_pos(glm::vec3(-3850.0f, 0.0f, 0.0f))
			.int_var("top_selection", 2)
		},
		{"Descriptions",
			SceneElement({
				{"Desc Bar", ScreenTexture("resource/scene/menu/main/DescBar.png")
					.set_orientation(TEXTURE_BOTTOM)
					.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
				},
				{"Desc Text", ScreenText(&get_font("desc_text"), "Placeholder", desc_spec)
					.set_orientation(TEXTURE_BOTTOM)
					.set_pos(glm::vec3(0.0f, 200.0f, 0.0f))
				},
			})
		}
	});
	for (int i = 1; i < 6; i++) {
		SceneElement& e = root.get_child(i).get_child("Sub Menu");
		e.get_screen_texture("Cursor")
			.set_pos(glm::vec3(100.0f, 200.0f, 0.0f))
			.set_screen_orientation(TEXTURE_TOP_LEFT)
			.set_texture_orientation(TEXTURE_LEFT)
			.set_base_width(50)
			.set_base_height(50);
		for (int j = 0; j < e.get_num_screen_texts(); j++) {
			e.get_screen_text(j)
				.set_pos(glm::vec3(200.0f, 200.0f + 125.0f * j, 0.0f))
				.set_screen_orientation(TEXTURE_TOP_LEFT)
				.set_texture_orientation(TEXTURE_LEFT);
		}
	}
	set_active_element(&get_element("root/Top Menu VS"));
}

MainMenu::~MainMenu() {

}

void MainMenu::render_main() {
	root.render();
}