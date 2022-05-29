#pragma warning(disable : 4996)
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <Windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <glew/glew.h>
#include "SDL Helpers.h"
#include "utils.h"
#include "GameSettings.h"
#include "GameStates.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "BattleObjectManager.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "ThreadManager.h"
#include "stb_image.h"
//Windows.h has a constant named LoadIcon, while Loader.h has a class named LoadIcon. C++ will always assume we mean the constant, so we need to 
//undefine it before we include Loader.h.
#undef LoadIcon
#include "Loader.h"
#undef main

bool debug = false;

std::mutex file_mutex;

int main() {
	//Hide the console window. It'll be a LONG time before we get to uncomment these lines

//	HWND windowHandle = GetConsoleWindow();
//	ShowWindow(windowHandle, SW_HIDE);

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
	}

	SDL_GameControllerEventState(SDL_ENABLE);

	RenderManager* render_manager = RenderManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();	
	GameManager* game_manager = GameManager::get_instance();

	opening_main();

	bool running = *game_manager->game_state != GAME_STATE_CLOSE;

	while (running) {
		render_manager->refresh_sdl_renderer();
		for (int i = 0; i < MAX_LAYERS; i++) {
			game_manager->looping[i] = true;
		}
		if (game_manager->game_main[*game_manager->game_state] != nullptr) {
			game_manager->game_main[*game_manager->game_state]();
		}
		else if (*game_manager->game_state != GAME_STATE_CLOSE) {
			char buffer[86];
			sprintf(buffer, "Error: Game State was %d (not GAME_STATE_CLOSE) but there was no associated function!", *game_manager->game_state);
			game_manager->player_info[0]->crash_reason = buffer;
			game_manager->game_main[GAME_STATE_DEBUG_MENU]();
		}

		if (*game_manager->game_state == GAME_STATE_CLOSE) {
			running = false;
		}
	}

	game_manager->destroy();
	render_manager->destroy();
	sound_manager->unload_all_sounds();

	SDL_Quit();

//	ShowWindow(windowHandle, SW_SHOW);
//	If we try to end the program without putting the window back up, it technically causes a crash. Program was about to end anyway so it doesn't really
//	matter but ehhhhhhhhh, clean exits are nice
	return 0;
//	Same goes for this line; causes a crash on exit if we return 0 normally which wouldn't normally be a problem but again, clean exits
//	Don't ask me why exit(0) is ok but return 0 isn't, I cannot answer that question.
}