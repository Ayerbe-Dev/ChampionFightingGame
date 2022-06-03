#pragma warning(disable : 4996)
#include "Menu.h"
#include <glew/glew.h>
#include "utils.h"
#include "Player.h"
#include <math.h>
#include "Debugger.h"
#include "DebugMenu.h"
#include "Options.h"
#include "Loader.h"
#include "RenderManager.h"

void menu_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];
	const Uint8* keyboard_state;


	MainMenu *main_menu = new MainMenu;
	main_menu->load_game_menu();

	while (*main_menu->looping) {
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
		main_menu->process_submenu_tables();
		main_menu->render();

		SDL_GL_SwapWindow(render_manager->window);

		if (main_menu->sub_state != GAME_SUBSTATE_NONE) {
			if (game_manager->game_substate_main[main_menu->sub_state] != nullptr) {
				game_manager->game_substate_main[main_menu->sub_state]();
			}
			else {
				game_manager->add_crash_log("Error: Game Substate was " + std::to_string(main_menu->sub_state) + " (not GAME_SUBSTATE_NONE) but there was no associated function!");
				game_manager->update_state(GAME_STATE_DEBUG_MENU);
			}
			main_menu->sub_state = GAME_SUBSTATE_NONE;
		}
	}

	delete main_menu;
}

MainMenu::MainMenu() {

}

MainMenu::~MainMenu() {
	for (int i = 0; i < 5; i++) {
		menu_items[i].destroy();
		sub_menu_tables[i]->destroy();
		delete sub_menu_tables[i];
	}
	delete game_loader;
}

void MainMenu::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->set_menu_info(this);

	game_loader = new GameLoader(1);
	std::thread loading_thread(LoadingScreen, (void*)game_loader);
	loading_thread.detach();

	background_texture.init("resource/ui/menu/main/bg.png");

	menu_items[0].init("resource/ui/menu/main/Online.png");
	menu_items[1].init("resource/ui/menu/main/Solo.png");
	menu_items[2].init("resource/ui/menu/main/VS.png", "resource/ui/menu/main/vsimg.png");
	menu_items[3].init("resource/ui/menu/main/Options.png");
	menu_items[4].init("resource/ui/menu/main/Extras.png");

	for (int i = 0; i < 5; i++) {
		menu_items[i].destination = i;
		sub_menu_tables[i] = new SubMenuTable(i);
	}

	sub_menu_tables[SUB_MENU_ONLINE]->item_count = 2;
	sub_menu_tables[SUB_MENU_ONLINE]->sub_text[0].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_ONLINE]->sub_text[1].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->item_count = 3;
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text[0].init("resource/ui/menu/main/Story.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text[1].init("resource/ui/menu/main/Arcade.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text[2].init("resource/ui/menu/main/Training.png");
	sub_menu_tables[SUB_MENU_VS]->item_count = 3;
	sub_menu_tables[SUB_MENU_VS]->sub_text[0].init("resource/ui/menu/main/PlayerVsPlayer.png");
	sub_menu_tables[SUB_MENU_VS]->sub_text[1].init("resource/ui/menu/main/PlayerVsCPU.png");
	sub_menu_tables[SUB_MENU_VS]->sub_text[2].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->item_count = 5;
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text[0].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text[1].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text[2].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text[3].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text[4].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->item_count = 4;
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text[0].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text[1].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text[2].init("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text[3].init("resource/ui/menu/main/Placeholder.png");
	for (int i = 0; i < 5; i++) {
		for (int i2 = 0; i2 < sub_menu_tables[i]->item_count; i2++) {
			sub_menu_tables[i]->sub_text[i2].set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		}
	}

	inc_thread();
	game_loader->finished = true;
}

void MainMenu::event_up_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection > -4) {
			top_selection--;
		}
		else {
			top_selection = 0;
			theta += 5 * offset;
		}
	}
	else {
		if (sub_menu_tables[sub_type]->selected_item == 0) {
			sub_menu_tables[sub_type]->selected_item = sub_menu_tables[sub_type]->item_count - 1;
		}
		else {
			sub_menu_tables[sub_type]->selected_item--;
		}
	}
}

void MainMenu::event_down_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection < 0) {
			top_selection++;
		}
		else {
			top_selection = -4;
			theta -= 5 * offset;
		}
	}
	else {
		if (sub_menu_tables[sub_type]->selected_item == sub_menu_tables[sub_type]->item_count - 1) {
			sub_menu_tables[sub_type]->selected_item = 0;
		}
		else {
			sub_menu_tables[sub_type]->selected_item++;
		}
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
	*game_state = GAME_STATE_DEBUG_MENU;
	*looping = false;
}

void MainMenu::event_select_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		menu_level = MENU_LEVEL_SUB;
		sub_type = menu_items[top_selection * -1].destination;
	}
	else {
		if (sub_type == SUB_MENU_OPTIONS) {
			sub_state = get_sub_selection(sub_type, sub_menu_tables[sub_type]->selected_item);
		}
		else {
			update_state(get_sub_selection(sub_type, sub_menu_tables[sub_type]->selected_item));
			*looping = false;
		}
	}
}

void MainMenu::event_back_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		update_state(GAME_STATE_TITLE_SCREEN);
		*looping = false;
	}
	else {
		menu_level = MENU_LEVEL_TOP;
	}
}

void MainMenu::process_background() {
	process_submenu_tables();
	render();
}

int get_sub_selection(int top_selection, int sub_selection) {
	int ret = GAME_STATE_DEBUG_MENU;
	switch (top_selection) {
		case(SUB_MENU_ONLINE): {
			switch (sub_selection) {
				case (SUB_ONLINE_LOBBY): {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				default: {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
			}
		} break;
		case(SUB_MENU_SINGLEPLAYER): {
			switch (sub_selection) {
				case (SUB_SINGLEPLAYER_STORY): {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				case (SUB_SINGLEPLAYER_ARCADE): {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				case (SUB_SINGLEPLAYER_TRAINING): {
					ret = GAME_STATE_CHARA_SELECT;
				} break;
				default: {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
			}
		} break;
		case(SUB_MENU_VS): {
			switch (sub_selection) {
				case(SUB_VS_PVP): {
					ret = GAME_STATE_CHARA_SELECT;
				} break;
				case(SUB_VS_PVC): {
					ret = GAME_STATE_CHARA_SELECT;
				} break;
				case(SUB_VS_TOURNAMENT): {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				default: {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
			}
		} break;
		case(SUB_MENU_OPTIONS): {
			switch (sub_selection) {
				case (SUB_OPTIONS_CONTROLS): {
					ret = GAME_SUBSTATE_CONTROLS;
				} break;
				default: {
					ret = GAME_SUBSTATE_NONE;
				} break;
			}
		} break;
		case(SUB_MENU_EXTRAS): {
			switch (sub_selection) {
				case (SUB_EXTRAS_SOUND_TEST): {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				case (SUB_EXTRAS_GALLERY): {
					ret = GAME_STATE_DEBUG_MENU;
				}
				default: {
					ret = GAME_STATE_DEBUG_MENU;
				} break;
			}
		} break;
	}
	return ret;
}

void MainMenu::render() {
	background_texture.render();
	menu_items[top_selection * -1].image_texture.process();
	menu_items[top_selection * -1].image_texture.render();

	//prebuffer render
	for (int i = 0; i < 5; i++) {
		menu_items[i].name_texture.set_pos(glm::vec3(int(magnitude * cos(theta + (i - 5) * offset)) + WINDOW_WIDTH / 4, int(magnitude * sin(theta + (i - 5) * offset)), 0.0));
		menu_items[i].name_texture.set_rot(glm::vec3(0.0, 0.0, ((theta + (i - 5) * offset) * 180) / 3.14));
		menu_items[i].name_texture.process();
		menu_items[i].name_texture.render();

	}

	//real render
	for (int i = 0; i < 5; i++) {
		menu_items[i].name_texture.set_pos(glm::vec3(int(magnitude * cos(theta + i * offset)) + WINDOW_WIDTH / 4, int(magnitude * sin(theta + i * offset)), 0.0));
		menu_items[i].name_texture.set_rot(glm::vec3(0.0, 0.0, ((theta + i * offset) * 180) / 3.14));
		menu_items[i].name_texture.process();
		menu_items[i].name_texture.render();
		sub_menu_tables[menu_items[top_selection * -1].destination]->table.process();
		sub_menu_tables[menu_items[top_selection * -1].destination]->cursor.process();
		sub_menu_tables[menu_items[top_selection * -1].destination]->table.render();
		sub_menu_tables[menu_items[top_selection * -1].destination]->cursor.render();
		for (int i2 = 0; i2 < sub_menu_tables[menu_items[top_selection * -1].destination]->item_count; i2++) {
			sub_menu_tables[menu_items[top_selection * -1].destination]->sub_text[i2].process();
			sub_menu_tables[menu_items[top_selection * -1].destination]->sub_text[i2].render();
		}
	}

	//postbuffer render
	for (int i = 0; i < 5; i++) {
		menu_items[i].name_texture.set_pos(glm::vec3(int(magnitude * cos(theta + (i + 5) * offset)) + WINDOW_WIDTH / 4, int(magnitude * sin(theta + (i + 5) * offset)), 0.0));
		menu_items[i].name_texture.set_rot(glm::vec3(0.0, 0.0, ((theta + (i + 5) * offset) * 180) / 3.14));
		menu_items[i].name_texture.process();
		menu_items[i].name_texture.render();
	}

	theta += ((top_selection * offset) - theta) / 16;
}

void MainMenu::process_submenu_tables() {
	for (int i = 0; i < 5; i++) {
		for (int i2 = 0; i2 < sub_menu_tables[i]->item_count; i2++) {
			sub_menu_tables[i]->sub_text[i2].set_pos(glm::vec3((float)WINDOW_WIDTH * 1.62, (float)WINDOW_HEIGHT * 0.5 + (i2 * 300 / sub_menu_tables[i]->item_count), 0.0));
		}
		sub_menu_tables[i]->cursor.set_pos(glm::vec3(sub_menu_tables[i]->sub_text[sub_menu_tables[i]->selected_item].pos.x - 83, sub_menu_tables[i]->sub_text[sub_menu_tables[i]->selected_item].pos.y - WINDOW_HEIGHT * 1.125, 0));
	}
}

MenuItem::MenuItem() {}
void MenuItem::init(std::string texture_dir, std::string texture_description_dir, int destination) {
	this->destination = destination;
	name_texture.init(texture_dir);
	name_texture.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
	name_texture.scale_all_percent(2.0, false);
	image_texture.init(texture_description_dir);
	image_texture.set_width_scale(1.5);
	image_texture.set_height_scale(1.5);
	image_texture.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}
void MenuItem::destroy() {
	name_texture.destroy();
	image_texture.destroy();
}

SubMenuTable::SubMenuTable() {}
SubMenuTable::SubMenuTable(int selection) {
	table.init("resource/ui/menu/main/SubMenu.png");
	table.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT);
	table.scale_top_percent(1.4, false);
	table.scale_bottom_percent(1.4, false);
	table.scale_top_percent(1.2, false);
	table.scale_bottom_percent(1.2, false);

	cursor.init("resource/ui/menu/main/Cursor.png");
	cursor.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	cursor.set_pos(glm::vec3(700.0, 0.0, 0.0));
	cursor.set_width(50);
	cursor.set_height(50);

	this->selection = selection;
	selected_item = 0;
}

void SubMenuTable::destroy() {
	table.destroy();
	cursor.destroy();
	for (int i = 0; i < item_count; i++) {
		sub_text[i].destroy();
	}
}