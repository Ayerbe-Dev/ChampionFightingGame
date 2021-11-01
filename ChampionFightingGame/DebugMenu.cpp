#include <iostream>
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "utils.h"
#include "DebugMenu.h"
#include "Debugger.h"
extern u32 tick;
extern u32 tok;



int debugMenu(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2], int gamestate) {
	printf("Enter Debug Menu\n");
	const Uint8* keyboard_state;
	std::ostringstream lastString;
	SDL_Texture* option_texts[8];
	int option_surfaces[8];
	SDL_Rect dest;
	bool debugging = true;
	bool bPressedDown = false;
	bool bPressedUp = false;
	SDL_Rect selectRect{ 0, 0, 100, 700 };
	Debugger debugger;
	debugger = Debugger();

	//new
	printf("Load Font\n");
	TTF_Font* pFont = loadDebugFont();
	printf("Create Debug List\n");
	DebugList debugList = {pRenderer,pFont};

	printf("Add Entry\n");
	debugList.addEntry("Opt1");
	debugList.addEntry("Opt2");
	debugList.addEntry("Opt3");
	debugList.addEntry("Opt4");
	debugList.addEntry("Opt5");
	debugList.addEntry("Opt6");
	//debugList.addEntry("omega beans");
	//debugList.addEntry("kod3 beans");
	//debugList.addEntry("OUR BEANS");
	
	//

	// option_surfaces[0] = 400;
	// lastString << "Menu Call [" << gamestate << "] 'SPACE' or 'ENTER' to select";
	// option_texts[0] = newFontTexture(lastString.str(), pRenderer, font);

	// option_surfaces[1] = 50;
	// option_texts[1] = newFontTexture("GAME", pRenderer, font);

	// option_surfaces[2] = 50;
	// option_texts[2] = newFontTexture("MENU", pRenderer, font);

	// option_surfaces[3] = 60;
	// option_texts[3] = newFontTexture("CSS", pRenderer, font);

	// option_surfaces[4] = 60;
	// option_texts[4] = newFontTexture("CLOSE", pRenderer, font);

	// option_surfaces[5] = 120;
	// option_texts[5] = newFontTexture("DEBUG (this)", pRenderer, font);

	// for (int i = 0; i < 2; i++) {
	// 	option_surfaces[i+6] = player_info[i].crash_length;
	// 	option_texts[i+6] = newFontTexture(player_info[i].crash_reason, pRenderer, font);
	// }

	SDL_SetRenderTarget(pRenderer,nullptr);
	while (debugging) {
		SDL_RenderClear(pRenderer);


		tok = SDL_GetTicks() - tick;
		if (tok < TICK_RATE_MS) {
			SDL_Delay(TICK_RATE_MS - tok);
		}
		tick = SDL_GetTicks();

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return GAME_STATE_CLOSE;
				}
				break;
			}
		}

		//rendering time

		debugList.render();
		
		// for (int i = 0; i <= 7; i++) {
		// 	dest = { 100, 26 * i, option_surfaces[i], 25 }; // I am so lazy oml
		// 	SDL_RenderCopy(pRenderer, option_texts[i], nullptr, &dest);
		// }

		// selectRect = { 0, 0, 100, 700 };
		// SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		// SDL_RenderFillRect(pRenderer, &selectRect);

		// SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		// selectRect = { 75, (select + 1) * 26 + 5, 15, 15 };
		// SDL_RenderFillRect(pRenderer, &selectRect);

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(nullptr);

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
			if ((&player_info[i])->check_button_trigger(BUTTON_DOWN)) {
				if (debugList.selection < GAME_STATE_MAX - 1) {
					debugList.selection++;
					printf("Selected: %d\n",debugList.selection);
				}
			}
			if ((&player_info[i])->check_button_trigger(BUTTON_UP)) {
				if (debugList.selection > 0) {
					printf("Selected: %d\n",debugList.selection);
					debugList.selection--;
				}
			}
			if ((&player_info[i])->check_button_trigger(BUTTON_START) || (&player_info[i])->check_button_trigger(BUTTON_LP)) {
				debugging = false;
			}
		}

		SDL_RenderPresent(pRenderer);
	}

	for (int i = 0; i <= GAME_STATE_MAX; i++) {
		SDL_DestroyTexture(option_texts[i]);
	}
	return debugList.selection;
}

TTF_Font* loadDebugFont(string fontname){
	TTF_Font* font = TTF_OpenFont(fontname.c_str(), DEBUG_MENU_FONT_SIZE);
	if (!font) {
		printf("Failed to load font:  %s\n", TTF_GetError());
	}
	return font;
}

DebugList::DebugList(){};
DebugList::DebugList(SDL_Renderer *pRenderer, TTF_Font *pFont){
	this->pRenderer = pRenderer;
	this->pFont = pFont;

	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		printf("%d\n",i);
		debugItems[i].preLoad(pRenderer,pFont);
	}
};

void DebugList::addEntry(string message, int selectable){
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_NOT_ACTIVE){
			
			debugItems[i].generateTexture(message);
			debugItems[i].destRect.y = ((DEBUG_MENU_FONT_SIZE+5) * i) + 10;
			debugItems[i].destRect.x = 0;

			debugItems[i].destRectSelect.y = debugItems[i].destRect.y;
			debugItems[i].destRectSelect.x = debugItems[i].destRect.x;

			debugItems[i].state = DEBUG_ITEM_ACTIVE;
			return;
		
		}
	}
}

void DebugList::render(){
	//printf("New Cycle\n");
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_ACTIVE){
				//printf("drawing:%d, with properites %d,%d,%d,%d\n",i,debugItems[i].destRect.x,debugItems[i].destRect.y,debugItems[i].destRect.w,debugItems[i].destRect.h);
				if(i == selection){
					SDL_RenderCopy(pRenderer,debugItems[i].pTextureSelect,nullptr,&debugItems[i].destRectSelect);
				} else {
					SDL_RenderCopy(pRenderer,debugItems[i].pTexture,nullptr,&debugItems[i].destRect);
				}
		}
	}
}

DebugItem::DebugItem(){};
void DebugItem::preLoad(SDL_Renderer *pRenderer, TTF_Font *pFont){
	this->pRenderer = pRenderer;
	this->pFont = pFont;
};

void DebugItem::generateTexture(string message){
	//
	SDL_Color sky = {204,247,255};
	SDL_Color red = { 179,0,59 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(pFont, message.c_str(), sky);
	SDL_Surface* textSurfaceSelect = TTF_RenderText_Solid(pFont, ("["+message+"]").c_str(), red);
	//
	if (!textSurface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}

	printf("Generating Normal...\n");
	
	//normal
	pTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
	
	SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);
	printf("%s\n", TTF_GetError());

	if(abs(destRect.w) > 1280){
		printf("WARNING: width exceeds window! real w %d\n", destRect.w);
	} else {
		printf("Normal Gen Success real w %d\n", destRect.w);
	}
	//

	//select
	pTextureSelect = SDL_CreateTextureFromSurface(pRenderer, textSurfaceSelect);
	SDL_QueryTexture(pTextureSelect,nullptr,nullptr,&destRectSelect.w,&destRectSelect.h);
	printf("%s\n", TTF_GetError());

	if(abs(destRect.w) > 1280){
		printf("WARNING: altwidth exceeds window! real w %d\n", destRectSelect.w);
	} else {
		printf("Alt Gen Success real w %d\n", destRectSelect.w);
	}

	SDL_FreeSurface(textSurfaceSelect);
	SDL_FreeSurface(textSurface);
}