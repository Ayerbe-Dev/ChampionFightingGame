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

void debug_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	ResourceManager* resource_manager = ResourceManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	font_manager->load_face("Fiend-Oblique");
	Mouse mouse;
	mouse.poll_buttons();
	glm::vec2 prev_mouse_pos = mouse.get_pos_flip_y();
	
	DebugMenu *debug = new DebugMenu;

	window_manager->update_shader_cams();
	window_manager->update_shader_lights();

	cotr_imgui_init();

	while (debug->looping) {
		game_manager->frame_delay_check_fps();
		window_manager->clear_screen();

		for (int i = 0; i < 2; i++) {
			game_manager->player[i]->controller.check_controllers();
		}
		mouse.poll_buttons();

		if (mouse.check_button_on(GLFW_MOUSE_BUTTON_1)) {
			glfwSetInputMode(window_manager->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glm::vec2 mouse_offset = mouse.get_pos_flip_y() - prev_mouse_pos;
			window_manager->camera.add_rot(mouse_offset.x * 0.2, mouse_offset.y * 0.2, 0.0f);
		}
		else {
			glfwSetInputMode(window_manager->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		prev_mouse_pos = mouse.get_pos_flip_y();

		window_manager->update_shader_cams();

		debug->process_game_state();
		debug->render_game_state();

		cotr_imgui_debug_dbmenu(debug);

		window_manager->update_screen();
	}
	cotr_imgui_terminate();
	glfwSetInputMode(window_manager->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	delete debug;
	resource_manager->unload_model("resource/chara/rowan/model/m0/model.fbx");
	resource_manager->unload_model("resource/chara/eric/model/m0/model.fbx");
	font_manager->unload_face("Fiend-Oblique");
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

	go2.model.load_model_instance("resource/chara/eric/model/m0/model.fbx");
	go2.model.load_textures("c1");
	go2.init_shader();
	go2.set_pos(glm::vec3(200.0, 0.0, 0.0));
	go2.set_rot(glm::vec3(0.0, 0.0, 90.0));
	go2.model.set_flip(true);

	tex.init("resource/game_state/chara_select/chara/rowan/render.png");
	text_field.init(0, 600, 80);
	text_field.set_pos(glm::vec3(0.0, -400.0, 0.0));
	text_field.attach_shader(ShaderManager::get_instance()->get_shader_switch_features(
		text_field.shader, 0, SHADER_FEAT_COLORMOD
	));
	text_field.set_colormod(glm::vec3(255.0, 255.0, 255.0));

	load_font("text_input", "FiraCode", 96);
	load_font("test_font", "Fiend-Oblique", 64);
	text.init(get_font("text_input"), "", glm::vec4(0.0, 0.0, 0.0, 255.0), glm::vec4(0.0));
	text.set_pos(glm::vec3(0.0, -400.0, 0.0));

	test_screentexture.init("resource/game_state/battle/ui/pause/overlay.png", TEX_FEAT_4T5V);
	test_worldtexture.init("resource/game_state/chara_select/chara/rowan/render.png", TEX_FEAT_4T5V).set_orientation(TEXTURE_BOTTOM).set_pos(glm::vec3(200.0, 0.0, 0.0));
	test_screentext.init(&get_font("test_font"), "This line is extremely long (relatively)\nThis one isn't\nThis line is extremely long (relatively)", TextSpecifier().color(glm::vec3(255.0, 0.0, 0.0)).border(4).centered(true).multiline_scroll(true));
	test_worldtext.init(&get_font("test_font"), "This line is extremely long (relatively)\nThis one isn't\nThis line is extremely long (relatively)", TextSpecifier().color(glm::vec3(255.0, 0.0, 0.0)).border(4).centered(true).multiline_scroll(true));
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
	Camera& camera = window_manager->camera;
	if (glfwGetKey(window_manager->window, GLFW_KEY_W)) {
		go1.add_pos(glm::vec3(0.0, 3.0, 0.0));
		camera.add_pos(0.0, 0.0, 1.0);
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_A)) {
		go1.add_pos(glm::vec3(-3.0, 0.0, 0.0));
		camera.add_pos(-1.0, 0.0, 0.0);

	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_S)) {
		go1.add_pos(glm::vec3(0.0, -3.0, 0.0));
		camera.add_pos(0.0, 0.0, -1.0);
	}

	if (glfwGetKey(window_manager->window, GLFW_KEY_D)) {
		go1.add_pos(glm::vec3(3.0, 0.0, 0.0));
		camera.add_pos(1.0, 0.0, 0.0);
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_Z)) {
		test_worldtexture.set_billboard_setting(BILLBOARD_ON);
		test_worldtext.set_billboard_setting(BILLBOARD_ON);
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_X)) {
		test_worldtexture.set_billboard_setting(BILLBOARD_OFF);
		test_worldtext.set_billboard_setting(BILLBOARD_OFF);
	}

	if (glfwGetKey(window_manager->window, GLFW_KEY_SPACE)) {
		test_worldtext.update_text("Updating text after space was pressed!").start_scroll(10);
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

	glDisable(GL_DEPTH_TEST);
	test_screentexture.render();
	test_screentext.render();
	test_worldtexture.render();
	test_worldtext.render();
	glEnable(GL_DEPTH_TEST);
}