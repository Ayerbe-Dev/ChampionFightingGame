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
#include "FontManager.h"

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	font_manager->load_face("Fiend-Oblique");
	Font font = font_manager->load_font("Fiend-Oblique", 256);
	int width;
	int height;
	unsigned int f1 = font.create_text("I kinda like this font but", glm::vec4(255.0, 0.0, 0.0, 255.0), &width, &height);
	GameTexture f1_texture;
	f1_texture.init(f1, width, height);
	f1_texture.add_pos(glm::vec3(0.0, 175.0, 0.0));
	unsigned int f2 = font.create_text("idk, might be too edgy. What", glm::vec4(0.0, 255.0, 0.0, 255.0), &width, &height);
	GameTexture f2_texture;
	f2_texture.init(f2, width, height);
	unsigned int f3 = font.create_text("do you think?", glm::vec4(0.0, 0.0, 255.0, 255.0), &width, &height);
	GameTexture f3_texture;
	f3_texture.init(f3, width, height);
	f3_texture.add_pos(glm::vec3(0.0, -175.0, 0.0));

	DebugMenu *debug = new DebugMenu;
	debug->load_game_menu();

	cotr_imgui_init();
	while (*debug->looping) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);
		cotr_imgui_debug_dbmenu(debug);
		f1_texture.render();
		f2_texture.render();
		f3_texture.render();
		SDL_GL_SwapWindow(render_manager->window);
	}
	cotr_imgui_terminate();
	font.unload_font();
	f1_texture.destroy();
	f2_texture.destroy();
	f3_texture.destroy();
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