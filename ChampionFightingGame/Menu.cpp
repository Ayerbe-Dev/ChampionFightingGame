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



int menu_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]) {
	Uint32 tick;
	Uint32 tok;
	Debugger debugger;
	debugger = Debugger();
	//neccesary for scaling
	SDL_Texture* pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	bool menuing = true;
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	SDL_Texture *bgTexture  = loadTexture("resource/ui/menu/main/funnybg.png",pRenderer);

	MenuItem menu_items[5];
	SubMenuTable *sub_menu_tables[5];
	menu_items[0] = MenuItem("resource/ui/menu/main/Online.png",pRenderer);
	menu_items[1] = MenuItem{"resource/ui/menu/main/Solo.png",pRenderer};
	menu_items[2] = MenuItem{"resource/ui/menu/main/VS.png",pRenderer, "resource/ui/menu/main/vsimg.png"};
	menu_items[3] = MenuItem{"resource/ui/menu/main/Options.png",pRenderer};
	menu_items[4] = MenuItem{"resource/ui/menu/main/Extras.png",pRenderer};
	for (int i = 0; i < 5; i++) {
		menu_items[i].destination = i;
		sub_menu_tables[i] = new SubMenuTable(i, pRenderer);
	}
	sub_menu_tables[SUB_MENU_ONLINE]->item_count = 2;
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->item_count = 3;
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Story.png", pRenderer);
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Arcade.png", pRenderer);
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Training.png", pRenderer);
	sub_menu_tables[SUB_MENU_VS]->item_count = 3;
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/PlayerVsPlayer.png", pRenderer);
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/PlayerVsCPU.png", pRenderer);
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_OPTIONS]->item_count = 5;
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[4] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_EXTRAS]->item_count = 4;
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png", pRenderer);
	SDL_Rect garborect = {0,0,232,32};

	float theta = 0;
	float offset = 3.14/13;
	float magnitude = WINDOW_WIDTH/2;  //this is about 45 degrees
	int top_selection = -2;
	int sub_selection = GAME_STATE_GAME;
	int menu_level = MENU_LEVEL_TOP;
	int sub_type = SUB_MENU_VS;

	while (menuing) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 100, 100, 100, 255);
		SDL_SetRenderTarget(pRenderer, pScreenTexture);
		SDL_RenderCopy(pRenderer,bgTexture,nullptr,nullptr);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
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
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(window, 0);
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
			}

			if (menu_level == MENU_LEVEL_TOP) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
					menuing = false;
					sub_selection = GAME_STATE_DEBUG_MENU;
				}

				if (player_info[i].check_button_trigger(BUTTON_MENU_SELECT)) {
					menu_level = MENU_LEVEL_SUB;
					sub_type = menu_items[top_selection * -1].destination;
					break;
				}
				if (player_info[i].check_button_trigger(BUTTON_DOWN)) {
					if (top_selection > -4) {
						top_selection--;
					}
					else {
						top_selection = 0;
						theta += 5 * offset;
					}
				}

				if (player_info[i].check_button_trigger(BUTTON_UP)) {
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
					break;
				}
				if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
					menu_level = MENU_LEVEL_TOP;
					break;
				}

				if (player_info[i].check_button_trigger(BUTTON_UP)) {
					if (sub_menu_tables[sub_type]->selected_item == 0) {
						sub_menu_tables[sub_type]->selected_item = sub_menu_tables[sub_type]->item_count - 1;
					}
					else {
						sub_menu_tables[sub_type]->selected_item--;
					}
				}
				if (player_info[i].check_button_trigger(BUTTON_DOWN)) {
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
			menu_items[i].destRect.x = int(magnitude*cos(theta+(i-5)*offset));
			menu_items[i].destRect.y = int(magnitude*sin(theta+(i-5)*offset)) + WINDOW_HEIGHT/2;

			SDL_RenderCopyEx(pRenderer,menu_items[i].texture,&garborect,&menu_items[i].destRect,((theta+(i-5)*offset)*180)/3.14,nullptr,flip);
		}

		//real render
		for (int i = 0; i < 5; i++) {
			menu_items[i].destRect.x = int(magnitude*cos(theta+i*offset));
			menu_items[i].destRect.y = int(magnitude*sin(theta+i*offset)) + WINDOW_HEIGHT/2;

			SDL_RenderCopyEx(pRenderer,menu_items[i].texture,&garborect,&menu_items[i].destRect,((theta+i*offset)*180)/3.14,nullptr,flip);
			SDL_RenderCopy(pRenderer, sub_menu_tables[menu_items[top_selection * -1].destination]->texture, NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->destRect);
			SDL_RenderCopy(pRenderer, sub_menu_tables[menu_items[top_selection * -1].destination]->cursor->texture, NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->cursor->destRect);
			for (int i2 = 0; i2 < sub_menu_tables[menu_items[top_selection * -1].destination]->item_count; i2++) {
				SDL_RenderCopy(pRenderer, sub_menu_tables[menu_items[top_selection * -1].destination]->sub_option_text[i2], NULL, &sub_menu_tables[menu_items[top_selection * -1].destination]->sub_option_rect[i2]);
			}
		}		

		//postbuffer render
		for (int i = 0; i < 5; i++) {
			menu_items[i].destRect.x = int(magnitude*cos(theta+(i+5)*offset));
			menu_items[i].destRect.y = int(magnitude * sin(theta + (i + 5) * offset)) + WINDOW_HEIGHT / 2;

			SDL_RenderCopyEx(pRenderer,menu_items[i].texture,&garborect,&menu_items[i].destRect,((theta+(i+5)*offset)*180)/3.14,nullptr,flip);
		}


		theta += ((top_selection * offset) - theta) / 16; 

		//printf("top_selection: %d, target theta: %f, theta: %f\n",top_selection,(top_selection * offset),theta);

		SDL_RenderCopy(pRenderer,menu_items[top_selection*-1].texture_description,nullptr,&menu_items[top_selection*-1].destRect_description);
		
		SDL_SetRenderTarget(pRenderer, nullptr);
		SDL_RenderCopy(pRenderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(pRenderer);
	}

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
					ret = GAME_STATE_GAME;
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

int chara_select_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]) {
	Uint32 tick=0,tok=0;
	Debugger debugger;
	debugger = Debugger();
	bool chara_selecting = true;
	int next_state;
	
	SDL_Texture* pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Texture* pCSSBackgroundTexture = loadTexture("resource/ui/menu/css/CSSbackground.png",pRenderer);
	SDL_Texture* pCSSBottomBarTexture = loadTexture("resource/ui/menu/css/CSSbottombar.png",pRenderer);

	const Uint8* keyboard_state;

	TTF_Font* pFont = loadDebugFont();
	CharaSelectSlot css_slots[32];
	int rows;
	int cols;
	int css_slot_count = load_css(css_slots, &rows, &cols, pRenderer);
	if (css_slot_count == -1) {
		player_info[0].crash_length = 500;
		player_info[0].crash_reason = "Could not open CSS file!";

		return GAME_STATE_DEBUG_MENU;
	}
	PlayerCursor player_cursor[2];
	PlayerCSSInfo player_css_info[2];
	for (int i = 0; i < 2; i++) {
		player_cursor[i] = PlayerCursor(pRenderer, &player_info[i], css_slots[0].textRect.x, css_slots[0].textRect.y);
		player_css_info[i] = PlayerCSSInfo(pRenderer, &player_info[i]);
	}

	while (chara_selecting) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
		SDL_SetRenderTarget(pRenderer, pScreenTexture);
		SDL_RenderClear(pRenderer);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return GAME_STATE_CLOSE;
				} break;
			}
		}

		//Frame delay

		frameTimeDelay(&tick,&tok);

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(window, 0);
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_controller();
			(&player_info[i])->update_buttons(keyboard_state);
			if (player_info[i].check_button_on(BUTTON_MENU_DOWN)) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_DOWN)) {
					player_info[i].stick_hold_v_timer = MENU_STICK_HOLD_TIMER;
				}
				else if (player_info[i].stick_hold_v_timer == 0) {
					player_info[i].stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;
				}
				else {
					player_info[i].stick_hold_v_timer--;
					goto SKIP_D;
				}
				if (!player_css_info[i].selected) {
					if (player_cursor[i].my_row != rows) {
						int prev_col = player_cursor[i].my_col;
						player_cursor[i].my_row++;
						find_nearest_css_slot(css_slots, css_slot_count, player_cursor[i].pos_x, &player_cursor[i]);
					}
				}
				SKIP_D:
				string msg = "hi fez";
			}
			if (player_info[i].check_button_on(BUTTON_MENU_UP)) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_UP)) {
					player_info[i].stick_hold_v_timer = MENU_STICK_HOLD_TIMER;
				}
				else if (player_info[i].stick_hold_v_timer == 0) {
					player_info[i].stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;
				}
				else {
					player_info[i].stick_hold_v_timer--;
					goto SKIP_U;
				}
				if (!player_css_info[i].selected) {
					if (player_cursor[i].my_row != 0) {
						int prev_col = player_cursor[i].my_col;
						player_cursor[i].my_row--;
						find_nearest_css_slot(css_slots, css_slot_count, player_cursor[i].pos_x, &player_cursor[i]);
					}
				}
				SKIP_U:
				string msg = "it's me sans undertale";
			}
			if (player_info[i].check_button_on(BUTTON_MENU_RIGHT)) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_RIGHT)) {
					player_info[i].stick_hold_h_timer = MENU_STICK_HOLD_TIMER;
				}
				else if (player_info[i].stick_hold_h_timer == 0) {
					player_info[i].stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
				}
				else {
					player_info[i].stick_hold_h_timer--;
					goto SKIP_R;
				}
				if (!player_css_info[i].selected) {
					player_cursor[i].prev_side = true;
					if (player_cursor[i].my_row != rows) {
						if (player_cursor[i].my_col != CHARAS_PER_ROW) {
							player_cursor[i].my_col++;
						}
					}
					else {
						if (player_cursor[i].my_col != cols - 1) {
							player_cursor[i].my_col++;
						}
					}
				}
				SKIP_R:
				string msg = "doo doo doo, doo";
			}
			if (player_info[i].check_button_on(BUTTON_MENU_LEFT)) {
				if (player_info[i].check_button_trigger(BUTTON_MENU_LEFT)) {
					player_info[i].stick_hold_h_timer = MENU_STICK_HOLD_TIMER;
				}
				else if (player_info[i].stick_hold_h_timer == 0) {
					player_info[i].stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
				}
				else {
					player_info[i].stick_hold_h_timer--;
					goto SKIP_L;
				}
				if (!player_css_info[i].selected) {
					player_cursor[i].prev_side = false;
					if (player_cursor[i].my_col != 0) {
						player_cursor[i].my_col--;
					}

				}
				SKIP_L:
				string msg = "it's called skip L because i skip all of my Ls";
			}
			if (player_info[i].check_button_trigger(BUTTON_MENU_SELECT)) {
				if (!player_css_info[i].selected) {
					if (css_slots[player_css_info[i].selected_slot].selectable) {
						player_css_info[i].selected = true;
					}
				}
			}
			if (player_info[i].check_button_trigger(BUTTON_MENU_BACK)) {
				if (player_css_info[i].selected) {
					player_css_info[i].selected = false;
				}
				else {
					next_state = GAME_STATE_MENU;
					chara_selecting = false;
				}
			}

			for (int i2 = 0; i2 < css_slot_count; i2++) {
				player_info[i].chara_kind = CHARA_KIND_ROY;
				if (player_cursor[i].my_col == css_slots[i2].my_col
				&& player_cursor[i].my_row == css_slots[i2].my_row) {
					player_cursor[i].pos_x = css_slots[i2].textRect.x - 30;
					player_cursor[i].pos_y = css_slots[i2].textRect.y + 10;
					player_cursor[i].destRect.x = player_cursor[i].pos_x;
					player_cursor[i].destRect.y = player_cursor[i].pos_y;
					player_info[i].chara_kind = css_slots[i2].chara_kind;
					player_css_info[i].selected_slot = i2;
					break;
				}
			}
			
			if (player_info[i].check_button_trigger(BUTTON_LK)) {
				player_info[i].stage_kind = "training_room";
			}
			if (player_info[i].check_button_trigger(BUTTON_MK)) {
				player_info[i].stage_kind = "training_room_old";
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		if (player_info[0].check_button_trigger(BUTTON_MENU_START) || player_info[1].check_button_trigger(BUTTON_MENU_START)) {
			if (player_css_info[0].selected && player_css_info[1].selected) {
				next_state = GAME_STATE_GAME;
				chara_selecting = false;
			}
		}

		//Background
		SDL_RenderCopy(pRenderer, pCSSBackgroundTexture, NULL, nullptr);
		SDL_RenderCopy(pRenderer, pCSSBottomBarTexture, NULL, nullptr);

		for (int i = 0; i < css_slot_count; i++) {
			SDL_RenderCopy(pRenderer, css_slots[i].texture, NULL, &css_slots[i].destRect);
			draw_text(pRenderer, "FiraCode-Regular.ttf", css_slots[i].chara_name, css_slots[i].textRect.x, css_slots[i].textRect.y, 24, 255, 255, 255, 255);
		}
		for (int i = 0; i < 2; i++) {
			SDL_RenderCopy(pRenderer, player_cursor[i].texture, NULL, &player_cursor[i].destRect);
			SDL_RenderCopy(pRenderer, player_css_info[i].texture, NULL, &player_css_info[i].destRect);
			if (!player_css_info[i].selected) {
				SDL_SetTextureAlphaMod(css_slots[player_css_info[i].selected_slot].texture, 127);
			}
			SDL_RenderCopy(pRenderer, css_slots[player_css_info[i].selected_slot].texture, NULL, &player_css_info[i].destRect);
			draw_text(pRenderer, "FiraCode-Regular.ttf", css_slots[player_css_info[i].selected_slot].chara_name, player_css_info[i].destRect.x + player_css_info[i].destRect.w / 2, player_css_info[i].destRect.y + player_css_info[i].destRect.h, 24, 255, 255, 255, player_css_info[i].selected ? 255 : 128);
			SDL_SetTextureAlphaMod(css_slots[player_css_info[i].selected_slot].texture, 255);
		}

		SDL_SetRenderTarget(pRenderer, nullptr);
		SDL_RenderCopy(pRenderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(pRenderer);
	}
	TTF_CloseFont(pFont);
	return next_state;
}

int load_css(CharaSelectSlot css[32], int *rows, int *cols, SDL_Renderer *renderer) {
	ifstream css_table;
	css_table.open("resource/ui/menu/css/css_param.yml");

	if (css_table.fail()) {
		css_table.close();
		return -1;
	}

	int col = 0;
	int row = 0;
	int chara_max = 0;
	string chara_name;
	for (int i = 0; css_table >> chara_name; i++) {
		int chara_kind;
		string chara_dir;
		bool selectable;

		css_table >> chara_kind >> chara_dir >> selectable;
		css[i] = CharaSelectSlot(renderer, chara_kind, chara_name, chara_dir, col, row, selectable);
		if (col == CHARAS_PER_ROW) {
			row++;
			col = 0;
		}
		else {
			col++;
		}
		chara_max++;
	}
	for (int i = 0; i < chara_max; i++) {
		int swidth;
		int sheight;
		SDL_QueryTexture(css[i].texture, NULL, NULL, &swidth, &sheight); 
		/*
			We want the actual dimensions of the base sprite so the proportions of our CSS slot will always be the same even regardless of how we
			calculated the width. The base sprite is tall and ugly so we aren't actually maintaining the BASE proportion, but we are maintaining
			a proportion.
		*/
		SDL_Rect css_rect;
		if (row == 0) { //If we only have one row, we can upscale all of the slots and get free extra vertical space for it
			css_rect.w = CSS_WIDTH / col;
			css_rect.x = (css[i].my_col * css_rect.w) + (WINDOW_WIDTH / 10);
		}
		else {
			//If we have multiple rows, we need all of our slots to be the same size no matter what row they're on
			css_rect.w = CSS_WIDTH / CHARAS_PER_ROW;
			if (css[i].my_row == row) { //If we're on the bottom row, we want to orient our positions to the center of the row
				css_rect.x = ((css[i].my_col + (float)(CHARAS_PER_ROW - col + 1) / (float)2.0) * css_rect.w) + (WINDOW_WIDTH / 15);
			}
			else { //If we're on a filled row, it's literally the same calculation as if we only have one row but not as wide
				css_rect.x = (css[i].my_col * css_rect.w) + (WINDOW_WIDTH / 15);
			}
		}
		int factor = swidth / css_rect.w;
		css_rect.h = sheight / (factor * 1.5);
		css_rect.y = css[i].my_row * 1.2 * css_rect.h;

		css[i].destRect = css_rect;

		//Now that we know where each CSS slot is, we can center the text at the bottom center of that slot

		css[i].textRect.x = css[i].destRect.x + css[i].destRect.w / 2;
		css[i].textRect.y = css[i].destRect.y + css[i].destRect.h * 1.1;
		css[i].textRect.w = 30;
		css[i].textRect.h = 30;
	}

	*rows = row;
	*cols = col;
	return chara_max;
}

void find_nearest_css_slot(CharaSelectSlot css[32], int slot_count, int pos_x, PlayerCursor *player_cursor) {
	int nearest = 0;
	int i = 10 * player_cursor->my_row;
	int lowest_distance = abs((css[i].textRect.x + css[i].textRect.w / 2) - pos_x);
	int prev_col = player_cursor->my_col;
	while (i < slot_count) {
		if (abs((css[i].textRect.x + css[i].textRect.w / 2) - pos_x) < lowest_distance) {
			lowest_distance = abs((css[i].textRect.x + css[i].textRect.w / 2) - pos_x);
			nearest = i;
		}
		i++;
	}
	if (player_cursor->prev_side && slot_count % 2 == 1 && nearest != slot_count - 1) {
		nearest++;
	}
	player_cursor->my_col = css[nearest].my_col;
	if (lowest_distance < 15 && prev_col != 0) {
		player_cursor->prev_side = !player_cursor->prev_side;
	}

	return;
}

MenuItem::MenuItem(){}
MenuItem::MenuItem(string texture_dir, SDL_Renderer *pRenderer, string texture_description_dir, int destination){
	this->texture = loadTexture(texture_dir.c_str(),pRenderer);
	this->destRect = {0,0,232,32};
	this->destination = destination;
	this->texture_description = loadTexture(texture_description_dir.c_str(),pRenderer);
	this->destRect_description = {0,0,520,720};
}

SubMenuTable::SubMenuTable() {}
SubMenuTable::SubMenuTable(int selection, SDL_Renderer* pRenderer) {
	SDL_Rect sub_rect;
	sub_rect.x = (WINDOW_WIDTH * 0.72);
	sub_rect.y = WINDOW_HEIGHT * 0.1;
	sub_rect.w = WINDOW_WIDTH * 0.25;
	sub_rect.h = WINDOW_HEIGHT * 0.75;
	this->destRect = sub_rect;
	this->texture = loadTexture("resource/ui/menu/main/SubMenu.png", pRenderer);
	this->selection = selection;
	this->cursor = new Cursor(pRenderer);
	selected_item = 0;
}

CharaSelectSlot::CharaSelectSlot() {
	this->chara_kind = CHARA_KIND_MAX;
}
CharaSelectSlot::CharaSelectSlot(SDL_Renderer* pRenderer, int chara_kind, string chara_name, string chara_dir, int my_col, int my_row, bool selectable) {
	this->chara_kind = chara_kind;
	this->chara_name = chara_name;
	this->my_col = my_col;
	this->my_row = my_row;
	this->texture = loadTexture(("resource/ui/menu/css/chara/" + chara_dir + "/render.png").c_str(), pRenderer);
}

Cursor::Cursor() {}
Cursor::Cursor(SDL_Renderer* pRenderer) {
	SDL_Rect cursor_rect;
	cursor_rect.x = (WINDOW_WIDTH * 0.75);
	cursor_rect.y = WINDOW_HEIGHT * 0.18;
	cursor_rect.w = 30;
	cursor_rect.h = 30;
	this->destRect = cursor_rect;
	this->texture = loadTexture("resource/ui/menu/main/Cursor.png", pRenderer);
}

PlayerCursor::PlayerCursor() {}
PlayerCursor::PlayerCursor(SDL_Renderer* pRenderer, PlayerInfo* player_info, int init_x, int init_y) {
	this->player_info = player_info;
	if (player_info->id == 0) {
		this->texture = loadTexture("resource/ui/menu/css/p1_cursor.png", pRenderer);
	}
	else {
		this->texture = loadTexture("resource/ui/menu/css/p2_cursor.png", pRenderer);
	}
	pos_x = init_x - 30;
	pos_y = init_y + 10;
	this->prev_side = false;
	this->my_col = 0;
	this->my_row = 0;
	SDL_Rect cursor_rect;
	cursor_rect.x = pos_x;
	cursor_rect.y = pos_y;
	cursor_rect.w = 60;
	cursor_rect.h = 60;
	this->destRect = cursor_rect;
}

PlayerCSSInfo::PlayerCSSInfo() {}
PlayerCSSInfo::PlayerCSSInfo(SDL_Renderer *pRenderer, PlayerInfo *player_info) {
	this->player_info = player_info;
	this->texture = loadTexture("resource/ui/menu/css/deck.png", pRenderer);
	this->selected = false;
	this->selected_slot = 0;
	destRect.w = 160;
	if (player_info->id == 0) {
		destRect.x = 30;
	}
	else {
		destRect.x = WINDOW_WIDTH - destRect.w - 30;
	}
	destRect.y = WINDOW_HEIGHT - 220;
	destRect.h = 200;
}