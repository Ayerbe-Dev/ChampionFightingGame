#pragma warning(disable : 4996)
#pragma warning(disable : 4244) //int to float
#pragma warning(disable : 4267) //size_t to int
#include <iostream>
#include <Windows.h>
#include <SDL/SDL.h>
#include <glew/glew.h>
#include "GameStates.h"
#include "ObjectManager.h"
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
#include "TargetVarManager.h"
#include "ThreadManager.h"
#undef main

int main() {
	//NOTE: Eventually we will want to hide the console window. When this happens, we need to go into
	//Properties->Linker->System and change the Sub System from CONSOLE to WINDOWS

	std::cout << std::fixed; //Set up some debug info
	std::cin.setf(std::ios::fixed, std::ios::floatfield);

	if (SDL_Init(SDL_INIT_EVERYTHING)) { //Make the game actually boot
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_GameControllerEventState(SDL_ENABLE);

	//Initialize all of the singletons

	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	font_manager->load_face("FiraCode");
	GameManager* game_manager = GameManager::get_instance();

	ObjectManager* object_manager = ObjectManager::get_instance();
	ControllerManager* controller_manager = ControllerManager::get_instance();
	EffectManager* effect_manager = EffectManager::get_instance();
	ParamAccessor* param_accessor = ParamAccessor::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	TargetVarManager* target_var_manager = TargetVarManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();

	opening_main();

	game_manager->player[0]->chara_kind = CHARA_KIND_ROWAN;
	game_manager->player[1]->chara_kind = CHARA_KIND_ERIC;

	while (game_manager->next_game_state != GAME_STATE_CLOSE) {
		shader_manager->reset_common_ubos();
		if (game_manager->game_main[game_manager->next_game_state] != nullptr) {
			game_manager->game_main[game_manager->next_game_state]();
		}
		else if (game_manager->next_game_state != GAME_STATE_CLOSE) {
			game_manager->add_crash_log("Error: Next Game State was " + std::to_string(game_manager->next_game_state) + " (not GAME_STATE_CLOSE) but there was no associated function!");
			game_manager->game_main[GAME_STATE_DEBUG_MENU]();
		}
	}

	//When we're done, kill all the things

	object_manager->destroy_instance();
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
	target_var_manager->destroy_instance();
	thread_manager->destroy_instance();

	SDL_Quit();

	return 0;
}