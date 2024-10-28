#pragma warning(disable : 4996)
#include "MainMenu.h"
#include <glew/glew.h>
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



		main_menu->process_game_state();
		main_menu->render_game_state();

		window_manager->update_screen();

		if (main_menu->sub_state != GAME_STATE_NONE) {
			if (game_manager->game_main[main_menu->sub_state] != nullptr) {
				game_manager->game_main[main_menu->sub_state]();
			}
			else {
				game_manager->add_crash_log("Error: Game State was " + std::to_string(main_menu->sub_state) + " (not GAME_SUBSTATE_NONE) but there was no associated function!");
				game_manager->update_state(GAME_STATE_DEBUG_MENU);
			}
			main_menu->sub_state = GAME_STATE_NONE;
		}
	}

	font_manager->unload_face("Fiend-Oblique");
	delete main_menu;
}

MainMenu::MainMenu() {
	theta = 0;
	offset = 3.14 / 11;
	magnitude = WINDOW_WIDTH / 1.125;
	turn_frames = 8;
	menu_frame = 0;

	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	FontManager* font_manager = FontManager::get_instance();
	Font main_text_font = font_manager->load_font("Fiend-Oblique", 144);
	Font sub_text_font = font_manager->load_font("Fiend-Oblique", 80);
	glm::vec4 rgba = { 255.0, 127.0, 0.0, 255.0 };
	glm::vec4 border_rgbs = { 1.0, 1.0, 1.0, 2.0 };

	menu_objects.reserve(2);
	push_menu_object("Background"); {
		push_menu_texture("bg", "resource/game_state/menu/main/bg.gif");
		push_menu_process_function([this](MenuObject* object) {
			object->get_texture("bg").set_sprite(menu_frame);
		});
		push_menu_activity_group("Game State Background", &top_selection, true, 5); {
			push_menu_child("Online Background"); {
				push_menu_texture("Online Demo", "resource/game_state/menu/main/missingno.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				//TODO: Add a description text after each demo image
			} pop_menu_stack();
			push_menu_child("Solo Background"); {
				push_menu_texture("Solo Demo", "resource/game_state/menu/main/missingno.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
			} pop_menu_stack();
			push_menu_child("VS Background"); {
				push_menu_texture("VS Demo", "resource/game_state/menu/main/vsimg.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
			} pop_menu_stack();
			push_menu_child("Options Background"); {
				push_menu_texture("Options Demo", "resource/game_state/menu/main/missingno.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
			} pop_menu_stack();
			push_menu_child("Extras Background"); {
				push_menu_texture("Extras Demo", "resource/game_state/menu/main/missingno.png");
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
			} pop_menu_stack();
		} pop_menu_stack();
		push_menu_child("Rotating Text", 5); {
			push_menu_pre_render_function([this](MenuObject* object) {
				for (int i = 0; i < 5; i++) {
					object->get_texture(i).set_pos(glm::vec3(int(magnitude * cos(theta + (i - 5) * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + (i - 5) * offset)), 0.0));
					object->get_texture(i).set_rot(glm::vec3(0.0, 0.0, ((theta + (i - 5) * offset) * 180) / 3.14));
					object->get_texture(i).render();
					object->get_texture(i).set_pos(glm::vec3(int(magnitude * cos(theta + i * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + i * offset)), 0.0));
					object->get_texture(i).set_rot(glm::vec3(0.0, 0.0, ((theta + i * offset) * 180) / 3.14));
				}
			});
			push_menu_post_render_function([this](MenuObject* object) {
				for (int i = 0; i < 5; i++) {
					object->get_texture(i).set_pos(glm::vec3(int(magnitude * cos(theta + (i + 5) * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + (i + 5) * offset)), 0.0));
					object->get_texture(i).set_rot(glm::vec3(0.0, 0.0, ((theta + (i + 5) * offset) * 180) / 3.14));
					object->get_texture(i).render();
				}
			});
			push_menu_texture("Online Label", main_text_font, "Online", rgba, border_rgbs);
			push_menu_texture("Solo Label", main_text_font, "Solo", rgba, border_rgbs);
			push_menu_texture("VS Label", main_text_font, "VS", rgba, border_rgbs);
			push_menu_texture("Options Label", main_text_font, "Options", rgba, border_rgbs);
			push_menu_texture("Extras Label", main_text_font, "Extras", rgba, border_rgbs);
		} pop_menu_stack();
	} pop_menu_stack();
	border_rgbs.a = 1.0;
	push_menu_object("Sub Table"); {
		object_stack.top()->set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
		object_stack.top()->set_pos(glm::vec3(glm::vec3(-450.0, 1080.0, 0.0)));
		push_menu_texture("table", "resource/game_state/menu/main/SubMenu.png"); {
			last_pushed_texture->set_height_scale(1.2);
		}
		push_menu_texture("cursor", "resource/game_state/menu/main/Cursor.png"); {
			last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
			last_pushed_texture->set_pos(glm::vec3(1550.0, 0.0, 0.0));
			last_pushed_texture->set_width(50);
			last_pushed_texture->set_height(50);
		}
		push_menu_activity_group("Sub Entries", &top_selection, true, 5); {
			push_menu_child("Sub Online", 3); {
				push_menu_int_var("Sub Selection", 0);
				push_menu_up_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection--;
					if (selection == -1) {
						selection += object->num_textures();
					}
				});
				push_menu_down_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection++;
					if (selection == object->num_textures()) {
						selection = 0;
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					switch (object->int_var("Sub Selection")) {
						case (0): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (1): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (2): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						default: {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
					}
				});
				push_menu_process_function([this](MenuObject* object) {
					GameTexture& texture = object->get_parent().get_texture("cursor");
					texture.set_pos(glm::vec3(texture.pos.get_val().x, object->get_texture(object->int_var("Sub Selection")).pos.get_val().y, 0));
				});
				push_menu_texture("Lobby Label", sub_text_font, "Lobby", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 520.0, 0.0));
				push_menu_texture("Queue Label", sub_text_font, "Queue", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 320.0, 0.0));
				push_menu_texture("Coach Label", sub_text_font, "Coach", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 120.0, 0.0));
			} pop_menu_stack();
			push_menu_child("Sub Solo", 3); {
				push_menu_int_var("Sub Selection", 0);
				push_menu_up_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection--;
					if (selection == -1) {
						selection += object->num_textures();
					}
				});
				push_menu_down_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection++;
					if (selection == object->num_textures()) {
						selection = 0;
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					switch (object->int_var("Sub Selection")) {
						case (0): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (1): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (2): {
							this->update_state(GAME_STATE_STAGE_SELECT, GAME_CONTEXT_TRAINING);
						} break;
						default: {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
					}
				});
				push_menu_process_function([this](MenuObject* object) {
					GameTexture& texture = object->get_parent().get_texture("cursor");
					texture.set_pos(glm::vec3(texture.pos.get_val().x, object->get_texture(object->int_var("Sub Selection")).pos.get_val().y, 0));
				});
				push_menu_texture("Story Label", sub_text_font, "Story", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 520.0, 0.0));
				push_menu_texture("Arcade Label", sub_text_font, "Arcade", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 320.0, 0.0));
				push_menu_texture("Training Label", sub_text_font, "Training", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 120.0, 0.0));
			} pop_menu_stack();
			push_menu_child("Sub VS", 3); {
				push_menu_int_var("Sub Selection", 0);
				push_menu_up_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection--;
					if (selection == -1) {
						selection += object->num_textures();
					}
				});
				push_menu_down_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection++;
					if (selection == object->num_textures()) {
						selection = 0;
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					switch (object->int_var("Sub Selection")) {
						case (0): {
							this->update_state(GAME_STATE_STAGE_SELECT, GAME_CONTEXT_NORMAL);
						} break;
						case (1): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (2): {
							this->update_state(GAME_STATE_STAGE_SELECT, GAME_CONTEXT_SPECIAL);
						} break;
						default: {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
					}
				});
				push_menu_process_function([this](MenuObject* object) {
					GameTexture& texture = object->get_parent().get_texture("cursor");
					texture.set_pos(glm::vec3(texture.pos.get_val().x, object->get_texture(object->int_var("Sub Selection")).pos.get_val().y, 0));
				});
				push_menu_texture("Battle Label", sub_text_font, "Battle", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 520.0, 0.0));
				push_menu_texture("Tournament Label", sub_text_font, "Tournament", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 320.0, 0.0));
				push_menu_texture("Special Label", sub_text_font, "Special Match", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 120.0, 0.0));
			} pop_menu_stack();
			push_menu_child("Sub Options", 5); {
				push_menu_int_var("Sub Selection", 0);
				push_menu_up_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection--;
					if (selection == -1) {
						selection += object->num_textures();
					}
				});
				push_menu_down_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection++;
					if (selection == object->num_textures()) {
						selection = 0;
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					switch (object->int_var("Sub Selection")) {
						case (0): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (1): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (2): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (3): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (4): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						default: {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
					}
				});
				push_menu_process_function([this](MenuObject* object) {
					GameTexture& texture = object->get_parent().get_texture("cursor");
					texture.set_pos(glm::vec3(texture.pos.get_val().x, object->get_texture(object->int_var("Sub Selection")).pos.get_val().y, 0));
				});
				push_menu_texture("Controls Label", sub_text_font, "Controls", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 520.0, 0.0));
				push_menu_texture("Graphics Label", sub_text_font, "Graphics", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 320.0, 0.0));
				push_menu_texture("Audio Label", sub_text_font, "Audio", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 120.0, 0.0));
				push_menu_texture("Placeholder Label", sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, -80.0, 0.0));
				push_menu_texture("Placeholder2 Label", sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, -280.0, 0.0));
			} pop_menu_stack();
			push_menu_child("Sub Gallery", 4); {
				push_menu_int_var("Sub Selection", 0);
				push_menu_up_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection--;
					if (selection == -1) {
						selection += object->num_textures();
					}
				});
				push_menu_down_event_function([this](MenuObject* object) {
					int& selection = object->int_var("Sub Selection");
					selection++;
					if (selection == object->num_textures()) {
						selection = 0;
					}
				});
				push_menu_select_event_function([this](MenuObject* object) {
					switch (object->int_var("Sub Selection")) {
						case (0): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (1): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (2): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						case (3): {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
						default: {
							this->update_state(GAME_STATE_DEBUG_MENU);
						} break;
					}
				});
				push_menu_process_function([this](MenuObject* object) {
					GameTexture& texture = object->get_parent().get_texture("cursor");
					texture.set_pos(glm::vec3(texture.pos.get_val().x, object->get_texture(object->int_var("Sub Selection")).pos.get_val().y, 0));
				});
				push_menu_texture("Gallery Label", sub_text_font, "Gallery", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 520.0, 0.0));
				push_menu_texture("Sound Test Label", sub_text_font, "Sound Test", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 320.0, 0.0));
				push_menu_texture("Placeholder3 Label", sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, 120.0, 0.0));
				push_menu_texture("Placeholder4 Label", sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), border_rgbs);
				last_pushed_texture->set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
				last_pushed_texture->set_pos(glm::vec3(1650.0, -80.0, 0.0));
			} pop_menu_stack();
		} pop_menu_stack();
	} pop_menu_stack();

	main_text_font.unload_font();
	sub_text_font.unload_font();
}

MainMenu::~MainMenu() {

}

void MainMenu::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	if (menu_level == MENU_LEVEL_TOP) {
		if (menu_frame > 0) {
			menu_frame--;
		}
	}
	else {
		if (menu_frame < 1) { //TODO: When a full spritesheet is implemented, replace 1 w the number of frames
			menu_frame++;
		}
	}
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
	}
}

void MainMenu::render_main() {
	render_menu_object("Background");
	render_menu_object("Sub Table");

	theta += ((top_selection * -offset) - theta) / turn_frames;
}

void MainMenu::process_background() {
	process_main();
	render_main();
}

void MainMenu::event_up_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection < 4) {
			top_selection++;
		}
		else {
			top_selection = 0;
			theta += 5 * offset;
		}
	}
	else {
		get_menu_object("Sub Table").get_activity_group("Sub Entries").children[top_selection].event_up_press();
	}
}

void MainMenu::event_down_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection > 0) {
			top_selection--;
		}
		else {
			top_selection = 4;
			theta -= 5 * offset;
		}
	}
	else {
		get_menu_object("Sub Table").get_activity_group("Sub Entries").children[top_selection].event_down_press();
	}
}

void MainMenu::event_left_press() {
	if (menu_level == MENU_LEVEL_SUB) {
		event_back_press();
	}
}

void MainMenu::event_right_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		event_select_press();
	}
}

void MainMenu::event_start_press() {
	update_state(GAME_STATE_DEBUG_MENU);
}

void MainMenu::event_select_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		get_menu_object("Sub Table").set_pos(glm::vec3(450.0, 1080.0, 0.0), 6);
		menu_level = MENU_LEVEL_SUB;
	}
	else {
		get_menu_object("Sub Table").get_activity_group("Sub Entries").children[top_selection].event_select_press();
	}
}

void MainMenu::event_back_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		update_state(GAME_STATE_TITLE_SCREEN);
	}
	else {
		get_menu_object("Sub Table").set_pos(glm::vec3(-450.0, 1080.0, 0.0), 6);
		menu_level = MENU_LEVEL_TOP;
	}
}