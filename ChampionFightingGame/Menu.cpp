#include "Menu.h"
#include "utils.h"
#include "PlayerInfo.h"
#include <SDL.h>
#include <math.h>
extern bool debug;
extern u32 frame_advance_ms;
extern u32 frame_advance_entry_ms;
extern u32 tick;
extern u32 tok;
extern int error_render;

int menu_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]) {
	bool menuing = true;
	
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	MenuItem menu_items[5];

	menu_items[0] = MenuItem{"resource\\ui\\menu\\Online.png",pRenderer};
	menu_items[1] = MenuItem{"resource\\ui\\menu\\SinglePlayer.png",pRenderer};
	menu_items[2] = MenuItem{"resource\\ui\\menu\\VS.png",pRenderer};
	menu_items[3] = MenuItem{"resource\\ui\\menu\\Options.png",pRenderer};
	menu_items[4] = MenuItem{"resource\\ui\\menu\\Extras.png",pRenderer};
	SDL_Rect garborect = {0,0,232,32};

	float theta = 0;
	float offset = 3.14/8;
	float magnitude = WINDOW_WIDTH/2;  //this is about 45 degrees
	int selection = -2;

	while (menuing) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 100, 100, 100, 255);

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

		if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(window, 0);
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_buttons(keyboard_state);
		}

		if (player_info[0].check_button_trigger(BUTTON_MENU_START)) {
			menuing = false;
		}

		if (player_info[0].check_button_trigger(BUTTON_DOWN)){
			printf("down pressed selection:%d sz:%d\n",selection,sizeof(menu_items));
			if (selection > -4){
				selection --;
			} else {
				selection = 0;
			}
		}

		if (player_info[0].check_button_trigger(BUTTON_UP)){
			printf("up pressed selection:%d sz:%d\n",selection,sizeof(menu_items));
			if (selection < 0){
				selection ++;
			} else {
				selection = -4;
			}
			
		}

		for (int i = 0; i < 5; i++) {
			menu_items[i].destRect.x = int(magnitude*cos(theta+i*offset));
			menu_items[i].destRect.y = int(magnitude*sin(theta+i*offset)) + WINDOW_HEIGHT/2;

			//cout << i << " " << menu_items[i].destRect.x << " "<< menu_items[i].destRect.y << endl;

			SDL_RenderCopyEx(pRenderer,menu_items[i].texture,&garborect,&menu_items[i].destRect,((theta+i*offset)*180)/3.14,nullptr,flip);
		}		


		theta += ((selection * offset) - theta) / 16;

		//printf("selection: %d, target theta: %f, theta: %f\n",selection,(selection * offset),theta);

		SDL_RenderPresent(pRenderer);
	}

	return menu_items[selection*-1].destination;
}

int chara_select_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]) {
	bool chara_selecting = true;
	
	const Uint8* keyboard_state;
	tick = SDL_GetTicks();

	while (chara_selecting) {
		SDL_RenderClear(pRenderer);
		SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

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

		if (keyboard_state[SDL_SCANCODE_ESCAPE]) {
			if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(window, 0);
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}

		for (int i = 0; i < 2; i++) {
			(&player_info[i])->update_buttons(keyboard_state);
			if (player_info[i].check_button_trigger(BUTTON_LP)) {
				player_info[i].chara_kind = CHARA_KIND_ROY;
			}
			if (player_info[i].check_button_trigger(BUTTON_MP)) {
				player_info[i].chara_kind = CHARA_KIND_ERIC;
			}
		}

		if (player_info[0].check_button_trigger(BUTTON_MENU_START)) {
			chara_selecting = false;
		}

		SDL_RenderPresent(pRenderer);
	}
	
	return GAME_STATE_GAME;
}

MenuItem::MenuItem(){};
MenuItem::MenuItem(string texture_dir, SDL_Renderer *pRenderer){
	this->texture = loadTexture(texture_dir.c_str(),pRenderer);
	this->destRect = {0,0,232,32};
	this->destination = 1234567;
};