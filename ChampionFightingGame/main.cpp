#pragma warning(disable : 4996)
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <Windows.h>
#include <SDL/SDL.h>
#include <glew/glew.h>
#include "SDL Helpers.h"
#include "utils.h"
#include "GameStates.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "AIManager.h"
#include "BattleObjectManager.h"
#include "ControllerManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "GameManager.h"
#include "ParamAccessor.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
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

	SDL_GameControllerEventState(SDL_ENABLE);

	//Initialize all of the singletons

	AIManager* ai_manager = AIManager::get_instance();
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	ControllerManager* controller_manager = ControllerManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	GameManager* game_manager = GameManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	ParamAccessor* param_accessor = ParamAccessor::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();

	font_manager->load_face("FiraCode");

	opening_main();

	bool running = game_manager->game_state != GAME_STATE_CLOSE;

	while (running) {
		for (int i = 0; i < MAX_LAYERS; i++) {
			game_manager->looping[i] = true;
		}
		if (game_manager->game_main[game_manager->game_state] != nullptr) {
			game_manager->game_main[game_manager->game_state]();
		}
		else if (game_manager->game_state != GAME_STATE_CLOSE) {
			game_manager->add_crash_log("Error: Game State was " + std::to_string(game_manager->game_state) + " (not GAME_STATE_CLOSE) but there was no associated function!");
			game_manager->game_main[GAME_STATE_DEBUG_MENU]();
		}

		if (game_manager->game_state == GAME_STATE_CLOSE) {
			running = false;
		}
	}

	ai_manager->destroy_instance();
	battle_object_manager->destroy_instance();
	controller_manager->destroy_instance();
	effect_manager->destroy_instance();
	font_manager->destroy_instance();
	game_manager->destroy_instance();
	param_accessor->destroy_instance();
	render_manager->destroy_instance();
	resource_manager->destroy_instance();
	save_manager->destroy_instance();
	shader_manager->destroy_instance();
	sound_manager->destroy_instance();
	thread_manager->destroy_instance();

	SDL_Quit();

//	ShowWindow(windowHandle, SW_SHOW);
//	If we try to end the program without putting the window back up, it technically causes a crash. 
//  Program was about to end anyway so it doesn't really matter but ehhhhhhhhh, clean exits are nice.
	return 0;
}