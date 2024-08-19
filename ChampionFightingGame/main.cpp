#pragma warning(disable : 4996)
#pragma warning(disable : 4244) //int to float
#pragma warning(disable : 4267) //size_t to int
#include <iostream>
#include <Windows.h>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include "GameStates.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "EffectManager.h"
#include "FontManager.h"
#include "GameManager.h"
#include "ParamAccessor.h"
#include "WindowManager.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "TargetVarManager.h"
#include "ThreadManager.h"

int main() {
	//NOTE: Eventually we will want to hide the console window. When this happens, we need to go into
	//Properties->Linker->System and change the Sub System from CONSOLE to WINDOWS

	std::cout << std::fixed; //Set up some debug info
	std::cin.setf(std::ios::fixed, std::ios::floatfield);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Initialize all of the singletons

	WindowManager* window_manager = WindowManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	font_manager->load_face("FiraCode");
	GameManager* game_manager = GameManager::get_instance();

	EffectManager* effect_manager = EffectManager::get_instance();
	InputManager* input_manager = InputManager::get_instance();
	ObjectManager* object_manager = ObjectManager::get_instance();
	ParamAccessor* param_accessor = ParamAccessor::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();
	ShaderManager* shader_manager = ShaderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	TargetVarManager* target_var_manager = TargetVarManager::get_instance();
	ThreadManager* thread_manager = ThreadManager::get_instance();

	opening_main();

	game_manager->player[0]->chara_kind = CHARA_KIND_ERIC;
	game_manager->player[1]->chara_kind = CHARA_KIND_ERIC;

	while (game_manager->next_game_state != GAME_STATE_CLOSE) {
		window_manager->reset_gl_environment();
		shader_manager->reset_common_ubos();
		if (game_manager->game_main[game_manager->next_game_state] != nullptr) {
			game_manager->game_main[game_manager->next_game_state]();
		}
		else if (game_manager->next_game_state != GAME_STATE_CLOSE) {
			game_manager->add_crash_log("Error: Next Game State was " + std::to_string(game_manager->next_game_state) + " (not GAME_STATE_CLOSE) but there was no associated function!");
			game_manager->game_main[GAME_STATE_DEBUG_MENU]();
		}
		save_manager->update_player_info();
		save_manager->update_save_data();
	}

	//When we're done, kill all the things

	effect_manager->destroy_instance();
	font_manager->destroy_instance();
	game_manager->destroy_instance();
	input_manager->destroy_instance();
	object_manager->destroy_instance();
	param_accessor->destroy_instance();
	window_manager->destroy_instance();
	resource_manager->destroy_instance();
	save_manager->destroy_instance();
	shader_manager->destroy_instance();
	sound_manager->destroy_instance();
	target_var_manager->destroy_instance();
	thread_manager->destroy_instance();

	glfwTerminate();
	return 0;
}