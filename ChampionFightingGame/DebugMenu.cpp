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
#include "Loader.h"

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;

void debugMenu(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	Debugger debugger;
	debugger = Debugger();


	DebugLoader* debug_loader = new DebugLoader;
	debug_loader->player_info[0] = player_info[0];
	debug_loader->player_info[1] = player_info[1];

	debug_loader->lastString << "This menu was called from the destination [" << game_manager->prev_game_state << "]";

	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool loading = true;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	std::ostringstream lastString;
	TTF_Font* debug_font = NULL;
	debug_list debug_list;

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadDebug, "Init.rar", (void*)debug_loader);
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
		int total_items = 16; //Don't ask me why but I made every single line of the debug menu count as a separate item to load
		loadingBar.setTargetPercent(((float)debug_loader->loaded_items / total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(mutex);

		if (debug_loader->finished) {
			if (!debug_loader->can_ret) {
				debug_font = debug_loader->debug_font;
				debug_list = debug_loader->debug_list;

				game_manager->set_menu_info(&debug_list);
			}
			debug_loader->can_ret = true;

			loading = false;
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (*game_manager->looping) {
		frameTimeDelay();
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
		}
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

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
			player_info[i]->poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();
		*game_manager->game_state = debug_list.getDestination();
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		debug_list.render();

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}

	player_info[0]->crash_reason = "Crash Message Goes Here";
	player_info[1]->crash_reason = "Crash Message Goes Here";
	TTF_CloseFont(debug_font);
	delete debug_loader;

//	return game_manager->update(player_info, *game_manager->game_state);
}

TTF_Font* loadDebugFont(string fontname){
	TTF_Font* font = TTF_OpenFont(fontname.c_str(), DEBUG_MENU_FONT_SIZE);
	if (!font) {
		printf("Failed to load font:  %s\n", TTF_GetError());
	}
	return font;
}

debug_list::debug_list(){};
debug_list::debug_list(TTF_Font *pFont, int x_offset){
	init(pFont,x_offset);
};

void debug_list::init(TTF_Font *pFont, int x_offset){
	this->pFont = pFont;
	this->x_offset = x_offset;

	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		debugItems[i].preLoad(pFont);
	}
}

void debug_list::addEntry(string message, int selectable, int destination){
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_NOT_ACTIVE){
			
			debugItems[i].generateTexture(message);
			debugItems[i].destRect.y = ((DEBUG_MENU_FONT_SIZE+5) * i) + 20;
			debugItems[i].destRect.x = x_offset;

			debugItems[i].destRectSelect.y = debugItems[i].destRect.y;
			debugItems[i].destRectSelect.x = debugItems[i].destRect.x;

			debugItems[i].state = DEBUG_ITEM_ACTIVE;
			debugItems[i].selectable = selectable;
			debugItems[i].destination = destination;
			return;
		
		}
	}
}

void debug_list::render(){
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_ACTIVE){
			if(i == selection){
				SDL_RenderCopy(g_renderer,debugItems[i].pTextureSelect,nullptr,&debugItems[i].destRectSelect);
			} else {
				SDL_RenderCopy(g_renderer,debugItems[i].pTexture,nullptr,&debugItems[i].destRect);
			}
		}
	}
}

void debug_list::event_down_press(){
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

void debug_list::event_up_press(){
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

int debug_list::getDestination(){
	return debugItems[selection].destination;
}

void debug_list::event_start_press(){
	*looping = false;
}

DebugItem::DebugItem(){};
void DebugItem::preLoad(TTF_Font *pFont){
	this->pFont = pFont;
};

void DebugItem::generateTexture(string message){
	SDL_LockMutex(mutex);
	SDL_Color sky = {204,247,255};
	SDL_Color red = { 179,0,59 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(pFont, message.c_str(), sky);
	SDL_Surface* textSurfaceSelect = TTF_RenderText_Solid(pFont, ("["+message+"]").c_str(), red);

	if (!textSurface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}
	
	pTexture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
	
	SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);

	pTextureSelect = SDL_CreateTextureFromSurface(g_renderer, textSurfaceSelect);
	SDL_QueryTexture(pTextureSelect,nullptr,nullptr,&destRectSelect.w,&destRectSelect.h);

	SDL_FreeSurface(textSurfaceSelect);
	SDL_FreeSurface(textSurface);
	SDL_UnlockMutex(mutex);
}