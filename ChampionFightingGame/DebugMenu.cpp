#include <stdbool.h>
#include <string>
#include <sstream>
#include "DebugMenu.h"
#include "cotr_imgui_debugger.h"
#include "WindowManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "ShaderManager.h"
#include "InputManager.h"
#include "HxAFile.h"
#include "ScreenTexture.h"

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	
	ScreenTexture test_screentexture("resource/game_state/battle/ui/pause/overlay.png", TEX_FEAT_CORNER_CROP);
	
	DebugMenu *debug = new DebugMenu;

	window_manager->update_shader_cams();
	window_manager->update_shader_lights();

	float percent_x[4] = { 1.0, 0.0, 1.0, 0.0 };
	float percent_y[4] = { 1.0, 1.0, 0.0, 0.0 };
	int target = 0;

	cotr_imgui_init();

	while (debug->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();

		for (int i = 0; i < 2; i++) {
			game_manager->player[i]->controller.check_controllers();
		}

		if (glfwGetKey(window_manager->window, GLFW_KEY_0)) {
			target = 0;
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_1)) {
			target = 1;
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_2)) {
			target = 2;
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_3)) {
			target = 3;
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_D)) {
			percent_x[target] = std::min(percent_x[target] + 0.05, 1.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_A)) {
			percent_x[target] = std::max(percent_x[target] - 0.05, 0.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_W)) {
			percent_y[target] = std::min(percent_y[target] + 0.05, 1.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_S)) {
			percent_y[target] = std::max(percent_y[target] - 0.05, 0.0);
		}

		test_screentexture.crop_top_right_corner(percent_x[0], percent_y[0]).crop_top_left_corner(1.0 - percent_x[1], percent_y[1])
			.crop_bottom_right_corner(percent_x[2], 1.0 - percent_y[2]).crop_bottom_left_corner(1.0 - percent_x[3], 1.0 - percent_y[3]);

		debug->process_game_state();
		debug->render_game_state();
		test_screentexture.render();

		cotr_imgui_debug_dbmenu(debug);

		window_manager->update_screen();
	}
	cotr_imgui_terminate();

	delete debug;
	test_screentexture.destroy();
	resource_manager->unload_model("resource/chara/rowan/model/m0/model.fbx");
	resource_manager->unload_model("resource/chara/eric/model/m0/model.fbx");
}

DebugMenu::DebugMenu() {
	GameManager* game_manager = GameManager::get_instance();

	std::string crash_reason;
	while (game_manager->get_crash_log(&crash_reason)) {
		debug_messages.push_back(crash_reason);
	}

	go1.model.load_model_instance("resource/chara/rowan/model/m0/model.fbx");
	go1.model.load_textures("c0");
	go1.init_shader();

	go1.set_pos(glm::vec3(-200.0, 0.0, 0.0));
	go1.set_rot(glm::vec3(0.0, 0.0, 90.0));

//	go1.load_anim_table("resource/chara/eric/anims/battle");
//	go1.change_anim("wait", 0.5, 0.0);


	go2.model.load_model_instance("resource/chara/eric/model/m0/model.fbx");
	go2.model.load_textures("c1");
	go2.init_shader();

	go2.set_pos(glm::vec3(200.0, 0.0, 0.0));
	go2.set_rot(glm::vec3(0.0, 0.0, 90.0));
//	go2.load_anim_table("resource/chara/eric/anims/battle");
//	go2.change_anim("wait", 0.5, 0.0);
	go2.model.set_flip(true);

	tex.init("resource/game_state/chara_select/chara/rowan/render.png");
	text_field.init(0, 600, 80);
	text_field.set_pos(glm::vec3(0.0, -400.0, 0.0));
	text_field.attach_shader(ShaderManager::get_instance()->get_shader_switch_features(
		text_field.shader, 0, SHADER_FEAT_COLORMOD
	));
	text_field.set_colormod(glm::vec3(255.0, 255.0, 255.0));

	load_font("text_input", "FiraCode", 96);
	text.init(get_font("text_input"), "", glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	text.set_pos(glm::vec3(0.0, -400.0, 0.0));
}

DebugMenu::~DebugMenu() {
	tex.destroy();
	go1.model.unload_textures();
	go2.model.unload_textures();
}

void DebugMenu::process_main() {
	go1.process_animate();
	go2.process_animate();
	WindowManager* window_manager = WindowManager::get_instance();
	if (glfwGetKey(window_manager->window, GLFW_KEY_W)) {
//		go1.add_pos(glm::vec3(0.0, 2.0, 0.0));
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_A)) {
//		go1.add_pos(glm::vec3(-2.0, 0.0, 0.0));
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_S)) {
//		go1.add_pos(glm::vec3(0.0, -2.0, 0.0));
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_D)) {
//		go1.add_pos(glm::vec3(2.0, 0.0, 0.0));
	}
}

void DebugMenu::render_main() {
	WindowManager* window_manager = WindowManager::get_instance();
	ShaderManager::get_instance()->set_global_float("Outline", 1.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);

	window_manager->shadow_map.use();
	glViewport(0, 0, 2000, 2000);
	glClear(GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_FRONT);
	go1.render_shadow();
	go2.render_shadow();
	glCullFace(GL_BACK);

	window_manager->g_buffer.use();
	glViewport(0, 0, window_manager->res_width, window_manager->res_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	window_manager->shadow_map.bind_textures();

	go1.render();
	go2.render();

	glDisable(GL_CULL_FACE);

	window_manager->render_ssao();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window_manager->window_width, window_manager->window_height);
	glEnable(GL_FRAMEBUFFER_SRGB);
	window_manager->g_buffer.render();
	glDisable(GL_FRAMEBUFFER_SRGB);
	tex.render();
	text_field.render();
	text.render();
}