#include "Menu.h"
#include "utils.h"
#include "PlayerInfo.h"
#include <SDL.h>
#include <math.h>
#include "Debugger.h"
#include "DebugMenu.h"
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern u32 tick;
extern u32 tok;
extern int error_render;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;


int menu_main(PlayerInfo player_info[2]) {
	displayLoadingScreen();
	Uint32 tick;
	Uint32 tok;
	Debugger debugger;
	debugger = Debugger();
	//neccesary for scaling
	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	bool menuing = true;

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	SDL_Texture* bgTexture = loadTexture("resource/ui/menu/main/funnybg.png");

	MenuItem menu_items[5];
	SubMenuTable* sub_menu_tables[5];
	menu_items[0] = MenuItem("resource/ui/menu/main/Online.png");
	menu_items[1] = MenuItem{ "resource/ui/menu/main/Solo.png"};
	menu_items[2] = MenuItem{ "resource/ui/menu/main/VS.png", "resource/ui/menu/main/vsimg.png" };
	menu_items[3] = MenuItem{ "resource/ui/menu/main/Options.png" };
	menu_items[4] = MenuItem{ "resource/ui/menu/main/Extras.png" };
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
	SDL_Rect garborect = { 0,0,232,32 };

	float theta = 0;
	float offset = 3.14 / 13;
	float magnitude = WINDOW_WIDTH / 2;  //this is about 45 degrees
	int top_selection = -2;
	int sub_selection = GAME_STATE_GAME;
	int menu_level = MENU_LEVEL_TOP;
	int sub_type = SUB_MENU_VS;

	while (menuing) {
		SDL_RenderClear(g_renderer);
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderCopy(g_renderer, bgTexture, nullptr, nullptr);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return GAME_STATE_CLOSE;
				} break;
			}
		}

		//Frame delay

		tok = SDL_GetTicks() - tick;
		if (tok < TICK_RATE_MS) {
			SDL_Delay(TICK_RATE_MS - tok);
		}
		tick = SDL_GetTicks();

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

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

		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_controller();
			(&player_info[i])->update_buttons(keyboard_state);
			if (player_info[i].check_button_trigger(BUTTON_MENU_START)) {
				menuing = false;
				sub_selection = GAME_STATE_DEBUG_MENU;
				goto SKIP_RENDER;
			}

			if (menu_level == MENU_LEVEL_TOP) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
					menuing = false;
					sub_selection = GAME_STATE_DEBUG_MENU;
					displayLoadingScreen();
					goto SKIP_RENDER;
				}

				if (player_info[i].check_button_trigger(BUTTON_MENU_SELECT)) {
					menu_level = MENU_LEVEL_SUB;
					sub_type = menu_items[top_selection * -1].destination;
					break;
				}
				if (player_info[i].vertical_input(true)) {
					if (top_selection > -4) {
						top_selection--;
					}
					else {
						top_selection = 0;
						theta += 5 * offset;
					}
				}

				if (player_info[i].vertical_input(false)) {
					if (top_selection < 0) {
						top_selection++;
					}
					else {
						top_selection = -4;
						theta -= 5 * offset;
					}
				}
			}

			if (menu_level == MENU_LEVEL_SUB) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_SELECT)) {
					menuing = false;
					displayLoadingScreen();
					goto SKIP_RENDER;
				}
				if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
					menu_level = MENU_LEVEL_TOP;
					break;
				}

				if (player_info[i].vertical_input(false)) {
					if (sub_menu_tables[sub_type]->selected_item == 0) {
						sub_menu_tables[sub_type]->selected_item = sub_menu_tables[sub_type]->item_count - 1;
					}
					else {
						sub_menu_tables[sub_type]->selected_item--;
					}
				}
				if (player_info[i].vertical_input(true)) {
					if (sub_menu_tables[sub_type]->selected_item == sub_menu_tables[sub_type]->item_count - 1) {
						sub_menu_tables[sub_type]->selected_item = 0;
					}
					else {
						sub_menu_tables[sub_type]->selected_item++;
					}
				}

				sub_selection = get_sub_selection(sub_type, sub_menu_tables[sub_type]->selected_item);
			}
		}

		for (int i = 0; i < 5; i++) {
			sub_menu_tables[i]->cursor->destRect.y = WINDOW_HEIGHT * 0.18 + (sub_menu_tables[i]->selected_item * 300 / sub_menu_tables[i]->item_count);
			for (int i2 = 0; i2 < sub_menu_tables[i]->item_count; i2++) {
				sub_menu_tables[i]->sub_option_rect[i2].x = WINDOW_WIDTH * 0.78;
				sub_menu_tables[i]->sub_option_rect[i2].y = WINDOW_HEIGHT * 0.18 + (i2 * 300 / sub_menu_tables[i]->item_count);
				sub_menu_tables[i]->sub_option_rect[i2].w = 200;
				sub_menu_tables[i]->sub_option_rect[i2].h = 32;
			}
		}

		//prebuffer render
		for (int i = 1; i < 5; i++) {
			menu_items[i].destRect.x = int(magnitude * cos(theta + (i - 5) * offset));
			menu_items[i].destRect.y = int(magnitude * sin(theta + (i - 5) * offset)) + WINDOW_HEIGHT / 2;

			SDL_RenderCopyEx(g_renderer, menu_items[i].texture, &garborect, &menu_items[i].destRect, ((theta + (i - 5) * offset) * 180) / 3.14, nullptr, flip);
		}

		//real render
		for (int i = 0; i < 5; i++) {
			menu_items[i].destRect.x = int(magnitude * cos(theta + i * offset));
			menu_items[i].destRect.y = int(magnitude * sin(theta + i * offset)) + WINDOW_HEIGHT / 2;

			SDL_RenderCopyEx(g_renderer, menu_items[i].texture, &garborect, &menu_items[i].destRect, ((theta + i * offset) * 180) / 3.14, nullptr, flip);
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

			SDL_RenderCopyEx(g_renderer, menu_items[i].texture, &garborect, &menu_items[i].destRect, ((theta + (i + 5) * offset) * 180) / 3.14, nullptr, flip);
		}


		theta += ((top_selection * offset) - theta) / 16;

		//printf("top_selection: %d, target theta: %f, theta: %f\n",top_selection,(top_selection * offset),theta);

		SDL_RenderCopy(g_renderer, menu_items[top_selection * -1].texture_description, nullptr, &menu_items[top_selection * -1].destRect_description);

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);

		checkLoadTime();
	}

	SKIP_RENDER:

	for (int i = 0; i < 5; i++) {
		delete sub_menu_tables[i]->cursor;
		delete sub_menu_tables[i];
	}

	SDL_DestroyTexture(pScreenTexture);
	SDL_DestroyTexture(bgTexture);

	return sub_selection;
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
					ret = GAME_STATE_DEBUG_MENU;
				} break;
				default: {
					ret = GAME_STATE_DEBUG_MENU;
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

MenuItem::MenuItem() {}
MenuItem::MenuItem(string texture_dir, string texture_description_dir, int destination) {
	this->texture = loadTexture(texture_dir.c_str());
	this->destRect = { 0,0,232,32 };
	this->destination = destination;
	this->texture_description = loadTexture(texture_description_dir.c_str());
	this->destRect_description = { 0,0,520,720 };
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
	cursor_rect.w = 30;
	cursor_rect.h = 30;
	this->destRect = cursor_rect;
	this->texture = loadTexture("resource/ui/menu/main/Cursor.png");
}