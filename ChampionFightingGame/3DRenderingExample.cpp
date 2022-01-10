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
	Shader shader_2("vertex_no_anim.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&shader_2);
//	Model model_1("resource/chara/roy/model/model.dae");
	Model model_2("resource/chara/roy/model/model.dae");
//	Animation3D test_anim("idle", "resource/chara/roy/anims/test.smd", &model_1);
	Shader tex_shader("vertex_sdl_overlay.glsl", "fragment_sdl_overlay.glsl");
	GameTextureNew p1_healthbar("resource/ui/game/hp/health.png");
	GameTextureNew p2_healthbar("resource/ui/game/hp/health.png");
	p1_healthbar.attach_shader(&tex_shader);
	p2_healthbar.attach_shader(&tex_shader);
	p1_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	p2_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
	p2_healthbar.flip_h();


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
			p1_healthbar.add_pos(vec3(0.0, 1.0, 0.0));
			p2_healthbar.add_pos(vec3(0.0, 1.0, 0.0));
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			p1_healthbar.add_pos(vec3(1.0, 0.0, 0.0));
			p2_healthbar.add_pos(vec3(1.0, 0.0, 0.0));
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			p1_healthbar.add_pos(vec3(-1.0, 0.0, 0.0));
			p2_healthbar.add_pos(vec3(-1.0, 0.0, 0.0));
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			p1_healthbar.add_pos(vec3(0.0, -1.0, 0.0));
			p2_healthbar.add_pos(vec3(0.0, -1.0, 0.0));
		}		
		if (player_info[1]->check_button_on(BUTTON_UP)) {
			p1_healthbar.scale_left_percent(0.8);
			p2_healthbar.scale_left_percent(0.8);
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) {
			p1_healthbar.scale_left_percent(1.0);
			p2_healthbar.scale_left_percent(1.0);
		}
		
//		if (frame == test_anim.length) {
//			frame = 0.0;
//		}
//		else {
//			frame += 1.0;
//		}
//		model_1.set_bones(frame, &test_anim);

		p1_healthbar.render();
		p2_healthbar.render();

//		g_rendermanager.render(&model_1, &shader_1, &model_pos, &model_rot, &model_scale);
		g_rendermanager.render(&model_2, &shader_2, &model_pos, &model_rot, &model_scale);

		SDL_GL_SwapWindow(g_window);
	}

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}