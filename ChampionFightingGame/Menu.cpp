#pragma warning(disable : 4996)
#include "Menu.h"
#include "utils.h"
#include "PlayerInfo.h"
#include <SDL.h>
#include <math.h>
#include "Debugger.h"
#include "DebugMenu.h"
#include "Options.h"
#include "Loader.h"

extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern int error_render;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;

void menu_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	Debugger debugger;
	debugger = Debugger();

	MenuLoader* menu_loader = new MenuLoader;

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool loading = true;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	MainMenu main_menu;
	MenuItem menu_items[5];

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadMenu, "Init.rar", (void*)menu_loader);
	SDL_DetachThread(loading_thread);

	game_manager->set_menu_info(nullptr);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (loading) {
		frameTimeDelay();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					*game_manager->game_state = GAME_STATE_CLOSE;
					return;
				}
				break;
			}
		}

		load_icon.move();
		SDL_LockMutex(mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		loadingBar.setTargetPercent(1, 0.3, 10);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(mutex);

		if (menu_loader->finished) {
			if (!menu_loader->can_ret) {
				main_menu = menu_loader->main_menu;
				game_manager->set_menu_info(&main_menu);
			}
			menu_loader->can_ret = true;

			loading = false;
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	while (*game_manager->looping[game_manager->layer]) {
		frameTimeDelay();
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
		}
		SDL_RenderClear(g_renderer);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}
		
		game_manager->handle_menus();
		main_menu.process_submenu_tables();

		SDL_SetRenderTarget(g_renderer, pScreenTexture);

		main_menu.render();

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);

		SDL_RenderPresent(g_renderer);

		if (main_menu.sub_state != GAME_SUBSTATE_NONE) {
			if (game_manager->game_substate_main[main_menu.sub_state] != nullptr) {
				game_manager->background[game_manager->layer] = pScreenTexture;
				game_manager->game_substate_main[main_menu.sub_state](game_manager);
			}
			else {
				char buffer[91];
				sprintf(buffer, "Error: Game Substate was %d (not GAME_SUBSTATE_NONE) but there was no associated function!", main_menu.sub_state);
				player_info[0]->crash_reason = buffer;
				*game_manager->looping[game_manager->layer] = false;
				game_manager->update_state(GAME_STATE_DEBUG_MENU);
			}
			main_menu.sub_state = GAME_SUBSTATE_NONE;
		}
	}

	SKIP_RENDER:

	SDL_DestroyTexture(pScreenTexture);

	for (int i = 0; i < 5; i++) {
		delete main_menu.sub_menu_tables[i]->cursor;
		delete main_menu.sub_menu_tables[i];
	}

	delete menu_loader;
}

void MainMenu::event_up_press() {
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
		if (top_selection > -4) {
			top_selection--;
		}
		else {
			top_selection = 0;
			theta += 5 * offset;
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

void MainMenu::process_background(SDL_Texture *background) {
	SDL_SetRenderTarget(g_renderer, background);
	SDL_RenderClear(g_renderer);
	process_submenu_tables();
	background_texture.render();
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

MainMenu::MainMenu(){}

void MainMenu::init(){
	background_texture.init("resource/ui/menu/main/bg.png");

	menu_items[0] = MenuItem("resource/ui/menu/main/Online.png");
	menu_items[1] = MenuItem{"resource/ui/menu/main/Solo.png"};
	menu_items[2] = MenuItem{"resource/ui/menu/main/VS.png", "resource/ui/menu/main/vsimg.png"};
	menu_items[3] = MenuItem{"resource/ui/menu/main/Options.png"};
	menu_items[4] = MenuItem{"resource/ui/menu/main/Extras.png"};

	for (int i = 0; i < 5; i++) {
		menu_items[i].destination = i;
		sub_menu_tables[i] = new SubMenuTable(i);
	}
	
	sub_menu_tables[SUB_MENU_ONLINE]->item_count = 2;
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->item_count = 3;
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Story.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Arcade.png");
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Training.png");
	sub_menu_tables[SUB_MENU_VS]->item_count = 3;
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/PlayerVsPlayer.png");
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/PlayerVsCPU.png");
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->item_count = 5;
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[4] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->item_count = 4;
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png");
};

void MainMenu::render() {
	background_texture.render();

	//prebuffer render
	for (int i = 1; i < 5; i++) {
		menu_items[i].destRect.x = int(magnitude * cos(theta + (i - 5) * offset));
		menu_items[i].destRect.y = int(magnitude * sin(theta + (i - 5) * offset)) + WINDOW_HEIGHT / 2;
		menu_items[i].destRect.y -= menu_items[i].destRect.h / 2;
		SDL_RenderCopyEx(g_renderer, menu_items[i].texture, nullptr, &menu_items[i].destRect, ((theta + (i - 5) * offset) * 180) / 3.14, nullptr, SDL_FLIP_NONE);

	}

	//real render
	for (int i = 0; i < 5; i++) {
		menu_items[i].destRect.x = int(magnitude * cos(theta + i * offset));
		menu_items[i].destRect.y = int(magnitude * sin(theta + i * offset)) + WINDOW_HEIGHT / 2;
		menu_items[i].destRect.y -= menu_items[i].destRect.h / 2;
		SDL_RenderCopyEx(g_renderer, menu_items[i].texture, nullptr, &menu_items[i].destRect, ((theta + i * offset) * 180) / 3.14, nullptr, SDL_FLIP_NONE);
		SDL_RenderCopy(g_renderer, sub_menu_tables[menu_items[top_selection * -1].destination]->texture, NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->destRect);
		SDL_RenderCopy(g_renderer, sub_menu_tables[menu_items[top_selection * -1].destination]->cursor->texture, NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->cursor->destRect);
		for (int i2 = 0; i2 < sub_menu_tables[menu_items[top_selection * -1].destination]->item_count; i2++) {
			SDL_RenderCopy(g_renderer, sub_menu_tables[menu_items[top_selection * -1].destination]->sub_option_text[i2], NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->sub_option_rect[i2]);
		}
	}

	//postbuffer render
	for (int i = 0; i < 5; i++) {
		menu_items[i].destRect.x = int(magnitude * cos(theta + (i + 5) * offset));
		menu_items[i].destRect.y = int(magnitude * sin(theta + (i + 5) * offset)) + WINDOW_HEIGHT / 2;

		SDL_RenderCopyEx(g_renderer, menu_items[i].texture, nullptr, &menu_items[i].destRect, ((theta + (i + 5) * offset) * 180) / 3.14, nullptr, SDL_FLIP_NONE);
	}


	theta += ((top_selection * offset) - theta) / 16;

	SDL_RenderCopy(g_renderer, menu_items[top_selection * -1].texture_description, nullptr, &menu_items[top_selection * -1].destRect_description);
}

void MainMenu::process_submenu_tables() {
	for (int i = 0; i < 5; i++) {
		sub_menu_tables[i]->cursor->destRect.y = WINDOW_HEIGHT * 0.18 + (sub_menu_tables[i]->selected_item * 300 / sub_menu_tables[i]->item_count);
		for (int i2 = 0; i2 < sub_menu_tables[i]->item_count; i2++) {
			sub_menu_tables[i]->sub_option_rect[i2].x = WINDOW_WIDTH * 0.78;
			sub_menu_tables[i]->sub_option_rect[i2].y = WINDOW_HEIGHT * 0.18 + (i2 * 300 / sub_menu_tables[i]->item_count);
			sub_menu_tables[i]->sub_option_rect[i2].w = 200;
			sub_menu_tables[i]->sub_option_rect[i2].h = 32;
		}
	}
}

MenuItem::MenuItem() {}
MenuItem::MenuItem(string texture_dir, string texture_description_dir, int destination) {
	this->texture = loadTexture(texture_dir.c_str());
	this->destRect = { 0,0, 348,48 };
	this->destination = destination;
	this->texture_description = loadTexture(texture_description_dir.c_str());
	this->destRect_description = { 0,0,780,1080 };
}

SubMenuTable::SubMenuTable() {}
SubMenuTable::SubMenuTable(int selection) {
	SDL_Rect sub_rect;
	sub_rect.x = (WINDOW_WIDTH * 0.72);
	sub_rect.y = WINDOW_HEIGHT * 0.1;
	sub_rect.w = WINDOW_WIDTH * 0.25;
	sub_rect.h = WINDOW_HEIGHT * 0.75;
	this->destRect = sub_rect;
	this->texture = loadTexture("resource/ui/menu/main/SubMenu.png");
	this->selection = selection;
	this->cursor = new Cursor();
	selected_item = 0;
}

Cursor::Cursor() {
	SDL_Rect cursor_rect;
	cursor_rect.x = (WINDOW_WIDTH * 0.75);
	cursor_rect.y = WINDOW_HEIGHT * 0.18;
	cursor_rect.w = 45;
	cursor_rect.h = 45;
	this->destRect = cursor_rect;
	this->texture = loadTexture("resource/ui/menu/main/Cursor.png");
}