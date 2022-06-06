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

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	DebugMenu *debug = new DebugMenu;
	debug->load_game_menu();

	cotr_imgui_init();
	while (*debug->looping) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);
		cotr_imgui_debug_dbmenu(debug);

		SDL_GL_SwapWindow(render_manager->window);
	}
	cotr_imgui_terminate();
	delete debug;
}

DebugMenu::DebugMenu() {

}

void DebugMenu::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->set_menu_info(this);

	std::string crash_reason;
	while (game_manager->get_crash_log(&crash_reason)) {
		debug_messages.push_back(crash_reason);
	}
}