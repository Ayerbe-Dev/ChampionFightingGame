#include <stdbool.h>
#include <string>
#include <sstream>
#include "DebugMenu.h"
#include "Debugger.h"
#include "Loader.h"
#include <glew/glew.h>
#include "RenderManager.h"
#include "FontManager.h"
#include "ResourceManager.h";

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();

	render_manager->reset_gl_environment();

	DebugMenu *debug = new DebugMenu;

	resource_manager->load_model("resource/chara/rowan/model/model.dae");

	GameObject go1;
	go1.model = resource_manager->use_model("resource/chara/rowan/model/model.dae");
	go1.model.load_textures("c0");
	go1.init_shader();

	GameObject go2;
	go2.model = resource_manager->use_model("resource/chara/rowan/model/model.dae");
	go2.model.load_textures("c1");
	go2.init_shader();

	go1.pos = glm::vec3(-200.0, 0, 0);
	go2.pos = glm::vec3(200.0, 0, 0);

	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();

	cotr_imgui_init();
	while (*debug->looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);

		go1.animate();

		glDepthMask(GL_TRUE);
		glEnable(GL_CULL_FACE);

		render_manager->shadow_map.use();
		glViewport(0, 0, 2000, 2000);
		glClear(GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
		go1.render_shadow();
		go2.render_shadow();
		glCullFace(GL_BACK);
		
		render_manager->g_buffer.use();
		glViewport(0, 0, render_manager->s_window_width, render_manager->s_window_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render_manager->shadow_map.bind_textures();

		go1.render();
		go2.render(); 

		glDisable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		render_manager->g_buffer.render();
		glDepthMask(GL_FALSE);

		cotr_imgui_debug_dbmenu(debug);
		
		SDL_GL_SwapWindow(render_manager->window);
	}
	cotr_imgui_terminate();
	go1.model.unload_textures();
	go2.model.unload_textures();
	resource_manager->unuse_model("resource/chara/rowan/model/model.dae");
	resource_manager->unuse_model("resource/chara/rowan/model/model.dae");
	resource_manager->unload_model("resource/chara/rowan/model/model.dae");
	render_manager->unlink_all_shaders();
	delete debug;
}

DebugMenu::DebugMenu() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->set_menu_info(this);

	std::string crash_reason;
	while (game_manager->get_crash_log(&crash_reason)) {
		debug_messages.push_back(crash_reason);
	}
}

DebugMenu::~DebugMenu() {

}