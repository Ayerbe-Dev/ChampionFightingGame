#include <stdbool.h>
#include <string>
#include <sstream>
#include "DebugMenu.h"
#include "cotr_imgui_debugger.h"
#include "GameManager.h"
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

		debug->process();
		debug->render();

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

	load_font("test_font", "Fiend-Oblique", 64);

	std::cout << "Begin ScreenText constructor sequence:\n";
	root.add_elements({
		{"Child", SceneElement({
			{"Screen", SceneElement({
				{"Texture", ScreenTexture("resource/scene/battle/ui/pause/overlay.png", TEX_FEAT_4T5V)
					.set_pos(glm::vec3(200.0f, 0.0f, 0.0f))
				},
				{"Text", ScreenText(
					&get_font("test_font"),
					"This line is extremely long (relatively)\nThis one isn't\nThis line is extremely long (relatively)",
					TextSpecifier()
						.color(glm::vec3(255.0, 0.0, 0.0))
						.border(4)
						.centered(true)
						.multiline_scroll(true)
					).set_pos(glm::vec3(2000.0f, 0.0f, 0.0f))
				}
			})},
			{"World", SceneElement({
				{"WorldTexture", WorldTexture("resource/scene/chara_select/chara/rowan/render.png", TEX_FEAT_4T5V)
					.set_orientation(TEXTURE_BOTTOM).set_pos(glm::vec3(200.0, 0.0, 0.0)) },
				{"WorldText", WorldText(
					&get_font("test_font"),
					"This line is extremely long (relatively)\nThis one isn't\nThis line is extremely long (relatively)",
					TextSpecifier()
						.color(glm::vec3(255.0, 0.0, 0.0))
						.border(4).centered(true)
						.multiline_scroll(true)
					)
				}
			})}
		}).add_event("up_press", [this](SceneElement* elem) {
			elem->get_world_text("World/WorldText").update_text("Testing the up event!").start_scroll(10);
		}).add_event("down_press", [this](SceneElement* elem) {
			elem->get_world_text("World/WorldText").update_text("Testing the down event!").start_scroll(10);
		})}
	});
	std::cout << "End ScreenText constructor sequence\n";
}

DebugMenu::~DebugMenu() {
	go1.model.unload_textures();
	go2.model.unload_textures();
}

void DebugMenu::process_main() {
	go1.process_animate();
	go2.process_animate();
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	Camera& camera = window_manager->camera;

	if (glfwGetKey(window_manager->window, GLFW_KEY_UP)) {
		root.get_child("Child").execute_event("up_press");
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_DOWN)) {
		root.get_child("Child").execute_event("down_press");
	}
	
	//When we're holding down M1, WASD allows the camera to move around (The camera's rotation is also 
	//controlled by the mouse in this state)
	if (mouse.check_button_on(GLFW_MOUSE_BUTTON_1)) {
		if (glfwGetKey(window_manager->window, GLFW_KEY_W)) {
			camera.add_pos(0.0, 0.0, 1.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_A)) {
			camera.add_pos(-1.0, 0.0, 0.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_S)) {
			camera.add_pos(0.0, 0.0, -1.0);
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_D)) {
			camera.add_pos(1.0, 0.0, 0.0);
		}
	}
	else {
		//Otherwise, our key inputs are for testing MenuObject hierarchal movement:
		//- WASD moves the child object around
		//- TFGH rotates the object which all SCREEN SPACE textures are part of
		//- IJKL rotates the object which all WORLD SPACE textures are part of

		if (glfwGetKey(window_manager->window, GLFW_KEY_W)) {
			root.get_child("Child").add_pos(glm::vec3(0.0, 1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_A)) {
			root.get_child("Child").add_pos(glm::vec3(-1.0, 0.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_S)) {
			root.get_child("Child").add_pos(glm::vec3(0.0, -1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_D)) {
			root.get_child("Child").add_pos(glm::vec3(1.0, 0.0, 0.0));
		}

		if (glfwGetKey(window_manager->window, GLFW_KEY_T)) {
			root.get_child("Child/Screen").add_rot(glm::vec3(0.0, 1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_F)) {
			root.get_child("Child/Screen").add_rot(glm::vec3(0.0, 0.0, -1.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_G)) {
			root.get_child("Child/Screen").add_rot(glm::vec3(0.0, -1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_H)) {
			root.get_child("Child/Screen").add_rot(glm::vec3(0.0, 0.0, 1.0));
		}

		if (glfwGetKey(window_manager->window, GLFW_KEY_I)) {
			root.get_child("Child/World").add_rot(glm::vec3(0.0, 1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_J)) {
			root.get_child("Child/World").add_rot(glm::vec3(0.0, 0.0, -1.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_K)) {
			root.get_child("Child/World").add_rot(glm::vec3(0.0, -1.0, 0.0));
		}
		if (glfwGetKey(window_manager->window, GLFW_KEY_L)) {
			root.get_child("Child/World").add_rot(glm::vec3(0.0, 0.0, 1.0));
		}
	}

	//Misc other scene tests

	if (glfwGetKey(window_manager->window, GLFW_KEY_Z)) {
		root.get_world_texture("Child/World/Texture").set_billboard_setting(BILLBOARD_ON);
		root.get_world_text("Child/World/Text").set_billboard_setting(BILLBOARD_ON);
		game_manager->fps_counter.add_width_scale(0.01).add_height_scale(0.01);
		for (int i = 0; i < window_manager->debug_textures.size(); i++) {
			window_manager->debug_textures[i].add_width_scale(0.01).add_height_scale(0.01);
		}
	}
	if (glfwGetKey(window_manager->window, GLFW_KEY_X)) {
		root.get_world_texture("Child/World/Texture").set_billboard_setting(BILLBOARD_OFF);
		root.get_world_text("Child/World/Text").set_billboard_setting(BILLBOARD_OFF);
		game_manager->fps_counter.add_width_scale(-0.01).add_height_scale(-0.01);
		for (int i = 0; i < window_manager->debug_textures.size(); i++) {
			window_manager->debug_textures[i].add_width_scale(-0.01).add_height_scale(-0.01);
		}
	}

	if (glfwGetKey(window_manager->window, GLFW_KEY_SPACE)) {
		root.get_world_text("Child/World/Text").update_text("Updating text after space was pressed!").start_scroll(10);
		root.get_screen_text("Child/Screen/Text").update_text("This line is extremely long (relatively)\nThis one isn't\nThis line is extremely long (relatively)");
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

	glDisable(GL_DEPTH_TEST);
	root.render();
	glEnable(GL_DEPTH_TEST);
}