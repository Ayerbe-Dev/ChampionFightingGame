#include <SDL/SDL.h>
#include <stdbool.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <sstream>
#include "DebugMenu.h"
#include "Debugger.h"
#include "Loader.h"
#include <glew/glew.h>
#include "RenderManager.h"

void debugMenu() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;

	std::ostringstream lastString;
	lastString << "This menu was called from the destination [" << *game_manager->prev_game_state << "]";

	SDL_SetRenderDrawBlendMode(render_manager->sdl_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(render_manager->sdl_renderer, 0, 0, 0, 255);

	SDL_Texture* pScreenTexture = SDL_CreateTexture(render_manager->sdl_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	TTF_Font* debug_font = loadDebugFont();
	debug_list debug_list(debug_font);

	game_manager->set_menu_info(nullptr);

	debug_list.addEntry("Welcome to the debug menu!", DEBUG_LIST_NOT_SELECTABLE);
	debug_list.addEntry("Use 'SPACE' or 'ENTER' to select an option.", DEBUG_LIST_NOT_SELECTABLE);
	debug_list.addEntry(lastString.str(), DEBUG_LIST_NOT_SELECTABLE);
	debug_list.addEntry("Title Screen", DEBUG_LIST_SELECTABLE, GAME_STATE_TITLE_SCREEN);
	debug_list.addEntry("Menu", DEBUG_LIST_SELECTABLE, GAME_STATE_MENU);
	debug_list.addEntry("Game", DEBUG_LIST_SELECTABLE, GAME_STATE_BATTLE);
	debug_list.addEntry("CSS", DEBUG_LIST_SELECTABLE, GAME_STATE_CHARA_SELECT);
	debug_list.addEntry("Debug (this menu)", DEBUG_LIST_SELECTABLE, GAME_STATE_DEBUG_MENU);
	debug_list.addEntry("Close", DEBUG_LIST_SELECTABLE, GAME_STATE_CLOSE);
	debug_list.addEntry(player_info[0]->crash_reason, DEBUG_LIST_NOT_SELECTABLE);
	debug_list.addEntry(player_info[1]->crash_reason, DEBUG_LIST_NOT_SELECTABLE);

	debug_list.event_down_press();

	game_manager->set_menu_info(&debug_list);

	SDL_SetRenderTarget(render_manager->sdl_renderer, pScreenTexture);
	SDL_RenderClear(render_manager->sdl_renderer);
	SDL_SetRenderTarget(render_manager->sdl_renderer, NULL);
	SDL_RenderCopy(render_manager->sdl_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	cotr_imgui_init();
	while (game_manager->looping[game_manager->layer]) {
		wait_ms();
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*SDL_SetRenderTarget(render_manager->sdl_renderer, pScreenTexture);
		SDL_RenderClear(render_manager->sdl_renderer);
		SDL_SetRenderTarget(render_manager->sdl_renderer, NULL);
		SDL_RenderCopy(render_manager->sdl_renderer, pScreenTexture, NULL, NULL);*/

		game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);

		//keyboard_state = SDL_GetKeyboardState(NULL);

		/*if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}*/

		/*for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}*/

		/*for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}*/

		/*game_manager->handle_menus();
		*game_manager->game_state = debug_list.getDestination();
		SDL_SetRenderTarget(render_manager->sdl_renderer, pScreenTexture);
		debug_list.render();

		SDL_SetRenderTarget(render_manager->sdl_renderer, nullptr);
		SDL_RenderCopy(render_manager->sdl_renderer, pScreenTexture, nullptr, nullptr);*/
		
		cotr_imgui_debug_dbmenu(game_manager);
		
		//SDL_RenderPresent(render_manager->sdl_renderer);
		SDL_GL_SwapWindow(render_manager->window);
	}
	cotr_imgui_terminate();
	player_info[0]->crash_reason = "Crash Message Goes Here";
	player_info[1]->crash_reason = "Crash Message Goes Here";
	TTF_CloseFont(debug_font);
	SDL_DestroyTexture(pScreenTexture);
	debug_list.destroy_list();

//	return game_manager->update(player_info, *game_manager->game_state);
}

TTF_Font* loadDebugFont(std::string fontname){
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

void debug_list::destroy_list() {
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++) {
		if (debugItems[i].state == DEBUG_ITEM_ACTIVE) {
			debugItems[i].delete_item();
		}
	}
}

void debug_list::init(TTF_Font *pFont, int x_offset){
	this->pFont = pFont;
	this->x_offset = x_offset;

	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		debugItems[i].preLoad(pFont);
	}
}

void debug_list::addEntry(std::string message, int selectable, int destination){
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
	RenderManager* render_manager = RenderManager::get_instance();
	for (int i = 0; i < DEBUG_MENU_ITEMS_MAX; i++){
		if (debugItems[i].state == DEBUG_ITEM_ACTIVE){
			if(i == selection){
				SDL_RenderCopy(render_manager->sdl_renderer,debugItems[i].pTextureSelect,nullptr,&debugItems[i].destRectSelect);
			} else {
				SDL_RenderCopy(render_manager->sdl_renderer,debugItems[i].pTexture,nullptr,&debugItems[i].destRect);
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

void DebugItem::delete_item() {
	SDL_DestroyTexture(pTexture);
	SDL_DestroyTexture(pTextureSelect);
}

void DebugItem::preLoad(TTF_Font *pFont){
	this->pFont = pFont;
};

void DebugItem::generateTexture(std::string message){
	RenderManager* render_manager = RenderManager::get_instance();
	file_mutex.lock();
	SDL_Color sky = {204,247,255};
	SDL_Color red = { 179,0,59 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(pFont, message.c_str(), sky);
	SDL_Surface* textSurfaceSelect = TTF_RenderText_Solid(pFont, ("["+message+"]").c_str(), red);

	if (!textSurface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}
	
	pTexture = SDL_CreateTextureFromSurface(render_manager->sdl_renderer, textSurface);
	
	SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);

	pTextureSelect = SDL_CreateTextureFromSurface(render_manager->sdl_renderer, textSurfaceSelect);
	SDL_QueryTexture(pTextureSelect,nullptr,nullptr,&destRectSelect.w,&destRectSelect.h);

	SDL_FreeSurface(textSurfaceSelect);
	SDL_FreeSurface(textSurface);
	file_mutex.unlock();
}