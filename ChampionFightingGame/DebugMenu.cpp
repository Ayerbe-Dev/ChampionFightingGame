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
	const Uint8* keyboard_state;
	std::ostringstream lastString;
	bool debugging = true;
	bool bPressedDown = false;
	bool bPressedUp = false;
	Debugger debugger;
	debugger = Debugger();

	//new
	TTF_Font* pFont = loadDebugFont();
	DebugList debugList = {pRenderer,pFont};
	lastString << "This menu was called from the destination [" << gamestate << "]";

	debugList.addEntry("Welcome to the debug menu!",DEBUG_LIST_NOT_SELECTABLE);
	debugList.addEntry("Use 'SPACE' or 'ENTER' to select an option.",DEBUG_LIST_NOT_SELECTABLE);
	debugList.addEntry(lastString.str(),DEBUG_LIST_NOT_SELECTABLE);
	debugList.addEntry("Menu", DEBUG_LIST_SELECTABLE, GAME_STATE_MENU);
	debugList.addEntry("Game", DEBUG_LIST_SELECTABLE, GAME_STATE_GAME);
	debugList.addEntry("CSS", DEBUG_LIST_SELECTABLE, GAME_STATE_CHARA_SELECT);
	debugList.addEntry(player_info[0].crash_reason,DEBUG_LIST_NOT_SELECTABLE);
	debugList.addEntry(player_info[1].crash_reason,DEBUG_LIST_NOT_SELECTABLE);
	debugList.addEntry("Debug (this menu)", DEBUG_LIST_SELECTABLE, GAME_STATE_DEBUG_MENU);
	debugList.addEntry("Close", DEBUG_LIST_SELECTABLE, GAME_STATE_CLOSE);
	

	SDL_SetRenderTarget(pRenderer,nullptr);

	//these make sure that the selector doesnt start on an unselectable row
	debugList.nextOption();
	debugList.previousOption();

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

		// end of rendering time, what a party
			//shot, shot, shot, lots of shot, shot, shot, only one shot, shot, shot, only one shot, and i've passed out

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
					debugList.nextOption();
			}
			if ((&player_info[i])->check_button_trigger(BUTTON_UP)) {
					debugList.previousOption();
			}
			if ((&player_info[i])->check_button_trigger(BUTTON_START) || (&player_info[i])->check_button_trigger(BUTTON_LP)) {
				debugging = false;
			}
		}

		SDL_RenderPresent(pRenderer);
	}
	(&player_info[0])->crash_reason = "Crash Message Goes Here";
	(&player_info[1])->crash_reason = "Crash Message Goes Here";
	TTF_CloseFont(pFont);
	return debugList.getDestination();
}

TTF_Font* loadDebugFont(string fontname){
	TTF_Font* font = TTF_OpenFont(fontname.c_str(), DEBUG_MENU_FONT_SIZE);
	if (!font) {
		printf("Failed to load font:  %s\n", TTF_GetError());
	}
	return font;
}

DebugList::DebugList(){};
DebugList::DebugList(SDL_Renderer *pRenderer, TTF_Font *pFont, int x_offset){
	this->pRenderer = pRenderer;
	this->pFont = pFont;
	this->x_offset = x_offset;

	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		debugItems[i].preLoad(pRenderer,pFont);
	}
};

void DebugList::addEntry(string message, int selectable, int destination){
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_NOT_ACTIVE){
			
			debugItems[i].generateTexture(message);
			debugItems[i].destRect.y = ((DEBUG_MENU_FONT_SIZE+5) * i) + x_offset;
			debugItems[i].destRect.x = 20;

			debugItems[i].destRectSelect.y = debugItems[i].destRect.y;
			debugItems[i].destRectSelect.x = debugItems[i].destRect.x;

			debugItems[i].state = DEBUG_ITEM_ACTIVE;
			debugItems[i].selectable = selectable;
			debugItems[i].destination = destination;
			return;
		
		}
	}
}

void DebugList::render(){
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_ACTIVE){
				if(i == selection){
					SDL_RenderCopy(pRenderer,debugItems[i].pTextureSelect,nullptr,&debugItems[i].destRectSelect);
				} else {
					SDL_RenderCopy(pRenderer,debugItems[i].pTexture,nullptr,&debugItems[i].destRect);
				}
		}
	}
}

void DebugList::nextOption(){
	//printf("next option\n");
	int pre = selection;
	selection ++;
	int i = selection;
	
	while(debugItems[i].selectable == DEBUG_LIST_NOT_SELECTABLE){
		i++;
	}	

	if (debugItems[i].state == DEBUG_ITEM_NOT_ACTIVE){
		selection = pre;
	} else {
		selection = i;
	}
}

void DebugList::previousOption(){
	//printf("previous option\n");
	int pre = selection;
	selection --;
	int i = selection;
	
	while(debugItems[i].selectable == DEBUG_LIST_NOT_SELECTABLE){
		i--;
	}

	if (i < 0){
		selection = pre;
	} else {
		selection = i;
	}
}

int DebugList::getDestination(){
	return debugItems[selection].destination;
}

DebugItem::DebugItem(){};
void DebugItem::preLoad(SDL_Renderer *pRenderer, TTF_Font *pFont){
	this->pRenderer = pRenderer;
	this->pFont = pFont;
};

void DebugItem::generateTexture(string message){
	SDL_Color sky = {204,247,255};
	SDL_Color red = { 179,0,59 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(pFont, message.c_str(), sky);
	SDL_Surface* textSurfaceSelect = TTF_RenderText_Solid(pFont, ("["+message+"]").c_str(), red);

	if (!textSurface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}
	
	pTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
	
	SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);

	pTextureSelect = SDL_CreateTextureFromSurface(pRenderer, textSurfaceSelect);
	SDL_QueryTexture(pTextureSelect,nullptr,nullptr,&destRectSelect.w,&destRectSelect.h);

	SDL_FreeSurface(textSurfaceSelect);
	SDL_FreeSurface(textSurface);
}