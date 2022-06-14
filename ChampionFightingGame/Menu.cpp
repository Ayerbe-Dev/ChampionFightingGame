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
#include "FontManager.h"
#include "SoundManager.h"

void menu_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	
	font_manager->load_face("Fiend-Oblique");

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
		main_menu->process_main();
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

	font_manager->unload_face("Fiend-Oblique");
	delete main_menu;
}

MainMenu::MainMenu() {
	theta = 0;
	offset = 3.14 / 11;
	magnitude = WINDOW_WIDTH / 1.125;
	turn_frames = 8;
	for (int i = 0; i < 5; i++) {
		sub_menu_tables[i] = nullptr;
	}
	menu_frame = 0;
}

MainMenu::~MainMenu() {
	for (int i = 0; i < 5; i++) {
		menu_items[i].destroy();
		sub_menu_tables[i]->destroy();
		delete sub_menu_tables[i];
	}
	table.destroy();
	delete game_loader;
}

void MainMenu::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->set_menu_info(this);

	RenderManager* render_manager = RenderManager::get_instance();

	game_loader = new GameLoader(1);
	std::thread loading_thread(&GameLoader::loading_screen, game_loader);
	loading_thread.detach();

	FontManager* font_manager = FontManager::get_instance();
	Font main_text_font = font_manager->load_font("Fiend-Oblique", 36);
	Font sub_text_font = font_manager->load_font("Fiend-Oblique", 20);

	table.init("resource/ui/menu/main/SubMenu.png");
	table.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
	table.set_pos(glm::vec3(WINDOW_WIDTH * 2, 0.0, 0.0));
	table.scale_top_percent(1.2, false);
	table.scale_bottom_percent(1.2, false);

	background_texture.init("resource/ui/menu/main/bg.png");
	background_texture.load_spritesheet("resource/ui/menu/main/bg.yml");

	menu_items[0].init(main_text_font, "Online", 0);
	menu_items[1].init(main_text_font, "Solo", 1);
	menu_items[2].init(main_text_font, "VS", 2, "resource/ui/menu/main/vsimg.png");
	menu_items[3].init(main_text_font, "Options", 3);
	menu_items[4].init(main_text_font, "Extras", 4);

	for (int i = 0; i < 5; i++) {
		sub_menu_tables[i] = new SubMenuTable(i);
	}

	sub_menu_tables[SUB_MENU_ONLINE]->sub_text.push_back(GameTexture(sub_text_font, "Lobby", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_ONLINE]->sub_text.push_back(GameTexture(sub_text_font, "Queue", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_ONLINE]->sub_text.push_back(GameTexture(sub_text_font, "Coach", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text.push_back(GameTexture(sub_text_font, "Story", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text.push_back(GameTexture(sub_text_font, "Arcade", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_text.push_back(GameTexture(sub_text_font, "Training", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_VS]->sub_text.push_back(GameTexture(sub_text_font, "Player VS Player", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_VS]->sub_text.push_back(GameTexture(sub_text_font, "Player VS CPU", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_VS]->sub_text.push_back(GameTexture(sub_text_font, "Tournament", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text.push_back(GameTexture(sub_text_font, "Controls", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text.push_back(GameTexture(sub_text_font, "Graphics", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text.push_back(GameTexture(sub_text_font, "Audio", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text.push_back(GameTexture(sub_text_font, "These all lead", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_text.push_back(GameTexture(sub_text_font, "to debug btw", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text.push_back(GameTexture(sub_text_font, "Gallery", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text.push_back(GameTexture(sub_text_font, "Sound Test", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text.push_back(GameTexture(sub_text_font, "fishing, probably", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_text.push_back(GameTexture(sub_text_font, "idk", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0));
	
	for (int i = 0; i < 5; i++) {
		for (int i2 = 0, max2 = sub_menu_tables[i]->sub_text.size(); i2 < max2; i2++) {
			sub_menu_tables[i]->sub_text[i2].set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		}
	}

	inc_thread();
	game_loader->finished = true;

	sub_text_font.unload_font();
}

void MainMenu::process_main() {
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
	background_texture.set_sprite(menu_frame);

	table.process();
	for (int i = 0; i < 5; i++) {
		for (int i2 = 0, max2 = sub_menu_tables[i]->sub_text.size(); i2 < max2; i2++) {
			sub_menu_tables[i]->sub_text[i2].set_pos(glm::vec3(table.pos.x + 160, (float)WINDOW_HEIGHT * 0.5 + (i2 * 130), 0.0));
		}
		sub_menu_tables[i]->cursor.set_pos(glm::vec3(table.pos.x + 80, sub_menu_tables[i]->sub_text[sub_menu_tables[i]->selected_item].pos.y - WINDOW_HEIGHT * 1.125, 0));
	}
}

void MainMenu::render() {
	background_texture.render();

	//prebuffer render
	for (int i = 0; i < 5; i++) {
		menu_items[i].sub_menu_name.set_pos(glm::vec3(int(magnitude * cos(theta + (i - 5) * offset)) - WINDOW_WIDTH / 1.385, int(magnitude * sin(theta + (i - 5) * offset)), 0.0));
		menu_items[i].sub_menu_name.set_rot(glm::vec3(0.0, 0.0, ((theta + (i - 5) * offset) * 180) / 3.14));
		menu_items[i].sub_menu_name.render();

	}

	//real render
	table.render();
	for (int i = 0; i < 5; i++) {
		menu_items[i].sub_menu_name.set_pos(glm::vec3(int(magnitude * cos(theta + i * offset)) - WINDOW_WIDTH / 1.385, int(magnitude * sin(theta + i * offset)), 0.0));
		menu_items[i].sub_menu_name.set_rot(glm::vec3(0.0, 0.0, ((theta + i * offset) * 180) / 3.14));
		menu_items[i].sub_menu_name.render();

		sub_menu_tables[menu_items[top_selection].destination]->cursor.render();
		for (int i2 = 0, max2 = sub_menu_tables[menu_items[top_selection].destination]->sub_text.size(); i2 < max2; i2++) {
			sub_menu_tables[menu_items[top_selection].destination]->sub_text[i2].render();
		}
	}

	//postbuffer render
	for (int i = 0; i < 5; i++) {
		menu_items[i].sub_menu_name.set_pos(glm::vec3(int(magnitude * cos(theta + (i + 5) * offset)) - WINDOW_WIDTH / 1.385, int(magnitude * sin(theta + (i + 5) * offset)), 0.0));
		menu_items[i].sub_menu_name.set_rot(glm::vec3(0.0, 0.0, ((theta + (i + 5) * offset) * 180) / 3.14));
		menu_items[i].sub_menu_name.render();
	}

	menu_items[top_selection].texture.render();

	theta += ((top_selection * -offset) - theta) / turn_frames;
}

void MainMenu::process_background() {
	process_main();
	render();
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
		if (sub_menu_tables[top_selection]->selected_item == 0) {
			sub_menu_tables[top_selection]->selected_item = sub_menu_tables[top_selection]->sub_text.size() - 1;
		}
		else {
			sub_menu_tables[top_selection]->selected_item--;
		}
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
		if (sub_menu_tables[top_selection]->selected_item == sub_menu_tables[top_selection]->sub_text.size() - 1) {
			sub_menu_tables[top_selection]->selected_item = 0;
		}
		else {
			sub_menu_tables[top_selection]->selected_item++;
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
		table.set_target_pos(glm::vec3(WINDOW_WIDTH * 1.535, 0.0, 0.0), 6);
		menu_level = MENU_LEVEL_SUB;
	}
	else {
		if (top_selection == SUB_MENU_OPTIONS) {
			sub_state = get_sub_selection(top_selection, sub_menu_tables[top_selection]->selected_item);
		}
		else {
			update_state(get_sub_selection(top_selection, sub_menu_tables[top_selection]->selected_item));
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
		table.set_target_pos(glm::vec3(WINDOW_WIDTH * 2, 0.0, 0.0), 6);
		menu_level = MENU_LEVEL_TOP;
	}
}

MenuItem::MenuItem() {
	destination = -1;
}

void MenuItem::init(Font font, std::string text, int destination, std::string texture_dir) {
	this->destination = destination;
	sub_menu_name.init(font, text, glm::vec4(255.0, 127.0, 0.0, 255.0), 12, -12);
	texture.init(texture_dir);
	texture.set_width_scale(1.5);
	texture.set_height_scale(1.5);
	texture.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

void MenuItem::destroy() {
	sub_menu_name.destroy();
	texture.destroy();
}

SubMenuTable::SubMenuTable() {
	selected_item = 0;
	selection = 0;
}

SubMenuTable::SubMenuTable(int selection) {
	cursor.init("resource/ui/menu/main/Cursor.png");
	cursor.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	cursor.set_pos(glm::vec3(700.0, 0.0, 0.0));
	cursor.set_width(50);
	cursor.set_height(50);

	this->selection = selection;
	selected_item = 0;
}

void SubMenuTable::destroy() {
	cursor.destroy();
	for (int i = 0, max = sub_text.size(); i < max; i++) {
		sub_text[i].destroy();
	}
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