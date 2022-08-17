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

	DebugMenu *debug = new DebugMenu;
	debug->load_game_menu();

	GameObject game_object;
//	game_object.load_model("resource/chara/roy/model/model.dae", "c0");
//	game_object.scale *= glm::vec3(0.05);

	render_manager->camera.set_fov(45.0);
	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();

	cotr_imgui_init();
	while (*debug->looping) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);

//		glDepthMask(GL_TRUE);
//		glEnable(GL_CULL_FACE);

//		render_manager->shadow_map.use();
//		glViewport(0, 0, 2000, 2000);
//		glClear(GL_DEPTH_BUFFER_BIT);

//		glCullFace(GL_FRONT);
//		game_object.render_shadow(false);
//		glCullFace(GL_BACK);
		
//		render_manager->g_buffer.use();
//		glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);

//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//		render_manager->shadow_map.bind_textures();

//		game_object.render();

//		glDisable(GL_CULL_FACE);

//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//		render_manager->g_buffer.render();
//		glDepthMask(GL_FALSE);

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