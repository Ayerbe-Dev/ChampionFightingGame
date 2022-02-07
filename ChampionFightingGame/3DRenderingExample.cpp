#include "3DRenderingExample.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "utils.h"
#include "stb_image.h"
#include "GameTexture.h"
#include <gtx/rotate_vector.hpp> 
#include <gtx/string_cast.hpp>

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

	Shader anim_shader("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&anim_shader);
	Shader model_shader("vertex_no_anim.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&model_shader);
	Model model("resource/chara/roy/model/model.dae");
	Model model_no_anim("resource/chara/roy/model/model.dae");
	Animation3D test_anim("idle", "resource/chara/roy/anims/ryu_shoryu.fbx", &model);
	GameTextureNew p1_healthbar("resource/ui/game/hp/health.png");
	GameTextureNew p2_healthbar("resource/ui/game/hp/health.png");
	p1_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	p2_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
	p2_healthbar.flip_h();


	vec3 model_pos = vec3(0.0, 0.0, 0.0);
	vec3 model_rot = vec3(0.0, 0.0, 0.0);
	vec3 model_scale = vec3(0.05, 0.05, 0.05);

	float frame = 0.0;
	int trials = 10000;

	int ticks = SDL_GetTicks();
	vector<int> average_ticks;
	vector<int> tick_frequency;
	vec3 total_rot(0.0);
	vec3 total_pos(0.0);
	while (three_deeing) {
		if (average_ticks.size() < trials) {
			average_ticks.push_back(SDL_GetTicks() - ticks);
		}
		else {
			int highest = average_ticks[0];
			while (highest >= tick_frequency.size()) {
				tick_frequency.push_back(0);
			}
			int freq = 0;
			int frame_freq = 0;
			float total = 0;
			for (int i = 0; i < trials; i++) {
				total += (float)average_ticks[i];
				if (average_ticks[i] >= 16) {
					frame_freq++;
				}
				if (average_ticks[i] > highest) {
					highest = average_ticks[i];
					while (highest >= tick_frequency.size()) {
						tick_frequency.push_back(0);
					}
					freq = 1;
				}
				else if (average_ticks[i] == highest) {
					freq++;
				}
				tick_frequency[average_ticks[i]]++;
			}
			total /= (float)trials;
			cout << "Lengths of all iterations across " << trials << " tests: " << endl;
			for (int i = 0; i < tick_frequency.size(); i++) {
				cout << "MS: " << i << ", Frequency: " << tick_frequency[i] << endl;
			}
			cout << "On average, it took " << total << " ms to run the loop, and there were " << frame_freq << " instances of an iteration taking more than a frame." << endl;
			average_ticks.clear();
			tick_frequency.clear();
		}
		frameTimeDelay();
		ticks = SDL_GetTicks();

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_Event event;
		vec3 rotation(0.0);
		vec3 pos(0.0);
		int target_bone = 93;
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
			g_rendermanager.camera.adjust_view(0.0, 1.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			g_rendermanager.camera.adjust_view(1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			g_rendermanager.camera.adjust_view(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			g_rendermanager.camera.adjust_view(0.0, -1.0, 0.0);
		}		
		if (player_info[1]->check_button_on(BUTTON_UP)) {
			g_rendermanager.camera.add_pos(0.0, 0.0, 1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) {
			g_rendermanager.camera.add_pos(0.0, 0.0, -1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) {
			g_rendermanager.camera.add_pos(1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) {
			g_rendermanager.camera.add_pos(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) {
			model_rot.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MP)) {
			model_rot.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) {
			model_rot.z += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LK)) {
			model_rot.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MK)) {
			model_rot.y -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HK)) {
			model_rot.z -= 0.1;
		}
		
		if (frame > test_anim.length) {
			frame = 0.0;
		}
		else {
			frame += 0.2;
		}

		model.set_bones(frame, &test_anim);
		model.set_bones(frame, &test_anim);

		g_rendermanager.render_model(&model, &anim_shader, &model_pos, &model_rot, &model_scale);
		g_rendermanager.render_model(&model_no_anim, &model_shader, &model_pos, &model_rot, &model_scale);

		p1_healthbar.render();
		p2_healthbar.render();

		SDL_GL_SwapWindow(g_window);
	}
	p1_healthbar.destroy();
	p2_healthbar.destroy();

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}