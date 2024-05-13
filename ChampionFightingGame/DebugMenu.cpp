#include <stdbool.h>
#include <string>
#include <sstream>
#include "DebugMenu.h"
#include "cotr_imgui_debugger.h"
#include <glew/glew.h>
#include "RenderManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "HxAFile.h"

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	
	render_manager->reset_gl_environment();

	DebugMenu *debug = new DebugMenu;

	render_manager->update_shader_cams();
	render_manager->update_shader_lights();
	render_manager->update_shader_shadows();

	cotr_imgui_init();
	while (debug->looping) {
		game_manager->frame_delay_check_fps();
		render_manager->clear_screen();

		for (int i = 0; i < 2; i++) {
			game_manager->player[i]->controller.check_controllers();
		}

		render_manager->handle_window_events(ImGui_ImplSDL2_ProcessEvent);

		debug->process_game_state();
		debug->render_game_state();

		cotr_imgui_debug_dbmenu(debug);
		
		render_manager->update_screen();
	}
	cotr_imgui_terminate();

	delete debug;
	resource_manager->unload_model("resource/chara/eric/model/m0/model.dae");
}

DebugMenu::DebugMenu() {
	GameManager* game_manager = GameManager::get_instance();

	std::string crash_reason;
	while (game_manager->get_crash_log(&crash_reason)) {
		debug_messages.push_back(crash_reason);
	}

	go1.model.load_model_instance("resource/chara/eric/model/m0/model.dae");
	go1.model.load_textures("c0");
	go1.init_shader();

	go1.pos = glm::vec3(-200.0, 0.0, 0.0);
	go1.rot.z = glm::radians(90.0);
	//	go1.load_anim_table("resource/chara/atlas/anims/battle");
	//	go1.change_anim("wait", 0.5, 0.0);


	go2.model.load_model_instance("resource/chara/eric/model/m0/model.dae");
	go2.model.load_textures("c1");
	go2.init_shader();

	go2.pos = glm::vec3(200.0, 0.0, 0.0);
	go2.rot.z = glm::radians(90.0);
	//	go2.load_anim_table("resource/chara/atlas/anims/battle");
	//	go2.change_anim("wait", 0.5, 0.0);
	go2.model.set_flip(true);

	tex.init("resource/game_state/chara_select/chara/rowan/render.png");
}

DebugMenu::~DebugMenu() {
	tex.destroy();
	go1.model.unload_textures();
	go2.model.unload_textures();
}

void DebugMenu::process_main() {
	go1.process_animate();
	go2.process_animate();
}

void DebugMenu::render_main() {
	RenderManager* render_manager = RenderManager::get_instance();
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
	glViewport(0, 0, render_manager->res_width, render_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	render_manager->shadow_map.bind_textures();

	go1.render();
	go2.render();

	glDisable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, render_manager->window_width, render_manager->window_height);
	render_manager->g_buffer.render();
	render_manager->gbuffer_texture.render();

	tex.render();
}