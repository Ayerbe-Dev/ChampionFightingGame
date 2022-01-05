#include "3DRenderingExample.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "utils.h"
#include "stb_image.h"

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

	Shader shader_1("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&shader_1);
	Shader shader_2("vertex_no_anim.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&shader_2);
	Model model_1("resource/chara/roy/model/model.dae");
	Model model_2("resource/chara/roy/model/model.dae");
	Animation3D test_anim("idle", "resource/chara/roy/anims/test.smd", &model_1);

	vec3 model_pos = vec3(0.0, -6.0, -6.0);
	vec3 model_rot = vec3(0.0, 0.0, 0.0);
	vec3 model_scale = vec3(0.05, 0.05, 0.05);

	int ticks = SDL_GetTicks();
	float frame = 0.0;
	float angle = 0.0;
	while (three_deeing) {
//		cout << "Time to execute this iteration of the loop: " << SDL_GetTicks() - ticks << endl;
		frameTimeDelay();
		ticks = SDL_GetTicks();

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

		if (player_info[0]->check_button_on(BUTTON_MENU_START)) {
			three_deeing = false;
		}
		if (player_info[0]->check_button_on(BUTTON_UP)) {
			model_pos.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			model_pos.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			model_pos.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			model_pos.y -= 0.1;
		}		
		if (player_info[1]->check_button_on(BUTTON_UP)) {
			model_pos.z += 0.1;
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) {
			model_pos.z -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) {
			model_rot.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LK)) {
			model_rot.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MP)) {
			model_rot.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MK)) {
			model_rot.y -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) {
			model_rot.z += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HK)) {
			model_rot.z -= 0.1;
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) {

		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) {

		}
		
		if (frame == test_anim.length) {
			frame = 0.0;
		}
		else {
			frame += 1.0;
		}
		model_1.set_bones(frame, &test_anim);

		g_rendermanager.render(&model_1, &shader_1, &model_pos, &model_rot, &model_scale);
//		g_rendermanager.render(&model_2, &shader_2, &model_pos, &model_rot, &model_scale);

		SDL_GL_SwapWindow(g_window);
	}

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}