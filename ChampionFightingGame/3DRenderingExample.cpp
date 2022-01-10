#include "3DRenderingExample.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "utils.h"
#include "stb_image.h"
#include "GameTexture.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern RenderManager g_rendermanager;
using namespace glm;

/// <summary>
/// Environment for learning and testing 3D rendering with OpenGL. Not to actually be used in the finished product.
/// </summary>
/// <param name="game_manager">: The GameManager instance that gets passed around everywhere.</param>
void three_d_rendering_main(GameManager* game_manager) {
	PlayerInfo* player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	bool three_deeing = true;

	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);

//	Shader shader_1("vertex_main.glsl", "fragment_main.glsl");
//	g_rendermanager.update_shader_lights(&shader_1);
//	Shader shader_2("vertex_no_anim.glsl", "fragment_main.glsl");
//	g_rendermanager.update_shader_lights(&shader_2);
//	Model model_1("resource/chara/roy/model/model.dae");
//	Model model_2("resource/chara/roy/model/model.dae");
//	Animation3D test_anim("idle", "resource/chara/roy/anims/test.smd", &model_1);
	Shader tex_shader("vertex_sdl_overlay.glsl", "fragment_sdl_overlay.glsl");
	GameTextureNew my_texture("resource/ui/game/hp/health.png");
	my_texture.attach_shader(&tex_shader);


	vec3 model_pos = vec3(0.0, -6.0, -6.0);
	vec3 model_rot = vec3(0.0, 0.0, 0.0);
	vec3 model_scale = vec3(0.05, 0.05, 0.05);

	float frame = 0.0;
	
	while (three_deeing) {
		frameTimeDelay();

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
						case SDL_WINDOWEVENT_MAXIMIZED: {
							int width;
							int height;
							SDL_GetWindowSize(g_window, &width, &height);
							glViewport(0, 0, width, height);
						} break;
					}
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

		if (player_info[0]->check_button_trigger(BUTTON_MENU_START)) {
			three_deeing = false;
		}
		if (player_info[0]->check_button_on(BUTTON_UP)) {
			my_texture.add_pos(vec3(0.0, 1.0, 0.0));
			model_pos.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			my_texture.add_pos(vec3(1.0, 0.0, 0.0));
			model_pos.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			my_texture.add_pos(vec3(-1.0, 0.0, 0.0));
			model_pos.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			my_texture.add_pos(vec3(0.0, -1.0, 0.0));
			model_pos.y -= 0.1;
		}		
		if (player_info[1]->check_button_trigger(BUTTON_UP)) {
			my_texture.scale_bottom_percent(0.8);
			model_pos.z += 0.1;
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) {
			my_texture.scale_bottom_percent(1.0);
			model_pos.z -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) {
			my_texture.flip_v();
			model_rot.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LK)) {
			my_texture.flip_h();
			model_rot.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MP)) {
			my_texture.scale_top_percent(0.6);
			model_rot.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MK)) {
			my_texture.scale_top_percent(1.0);
			model_rot.y -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) {
			my_texture.scale_right_percent(0.4);
			model_rot.z += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HK)) {
			my_texture.scale_right_percent(1.0);
			model_rot.z -= 0.1;
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) {
			my_texture.scale_left_percent(0.2, false);
		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) {
			my_texture.scale_left_percent(1.0, false);
		}
		if (keyboard_state[SDL_SCANCODE_0]) {
			my_texture.set_orientation(0);
		}
		if (keyboard_state[SDL_SCANCODE_1]) {
			my_texture.set_orientation(1);
		}
		if (keyboard_state[SDL_SCANCODE_2]) {
			my_texture.set_orientation(2);
		}
		if (keyboard_state[SDL_SCANCODE_3]) {
			my_texture.set_orientation(3);
		}
		if (keyboard_state[SDL_SCANCODE_4]) {
			my_texture.set_orientation(4);
		}
		if (keyboard_state[SDL_SCANCODE_5]) {
			my_texture.set_orientation(5);
		}
		if (keyboard_state[SDL_SCANCODE_6]) {
			my_texture.set_orientation(6);
		}
		if (keyboard_state[SDL_SCANCODE_7]) {
			my_texture.set_orientation(7);
		}
		if (keyboard_state[SDL_SCANCODE_8]) {
			my_texture.set_orientation(8);
		}
		
//		if (frame == test_anim.length) {
//			frame = 0.0;
//		}
//		else {
//			frame += 1.0;
//		}
//		model_1.set_bones(frame, &test_anim);

		my_texture.render();

//		g_rendermanager.render(&model_1, &shader_1, &model_pos, &model_rot, &model_scale);
//		g_rendermanager.render(&model_2, &shader_2, &model_pos, &model_rot, &model_scale);

		SDL_GL_SwapWindow(g_window);
	}

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}