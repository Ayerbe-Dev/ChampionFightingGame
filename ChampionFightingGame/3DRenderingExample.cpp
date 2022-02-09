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
#include "RenderObject.h"

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

	RenderObject p1;
	RenderObject p2;

	p1.shader.init("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&p1.shader);
	p1.model.load_model("resource/chara/roy/model/model.dae");

	p2.shader.init("vertex_main.glsl", "fragment_main.glsl");
	g_rendermanager.update_shader_lights(&p2.shader);
	p2.model.load_model("resource/chara/roy/model/model.dae");

	p1.scale = vec3(0.05);
	p2.scale = vec3(0.05);

	Animation3D anim[2];
	anim[0] = Animation3D("idle", "resource/chara/roy/anims/wait.fbx", &p1.model);
	anim[1] = Animation3D("idle", "resource/chara/roy/anims/wait.fbx", &p2.model);

	GameTextureNew p1_healthbar("resource/ui/game/hp/health.png");
	GameTextureNew p2_healthbar("resource/ui/game/hp/health.png");
	p1_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	p2_healthbar.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
	p2_healthbar.flip_h();


	float frame[2] = { 0.0 };
	int trials = 10000;

	int ticks = SDL_GetTicks();
	vector<int> average_ticks;
	vector<int> tick_frequency;
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
			p1.pos.y += p1.scale.y;
//			g_rendermanager.camera.adjust_view(0.0, 1.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			p1.pos.x += p1.scale.x;
//			g_rendermanager.camera.adjust_view(1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			p1.pos.x -= p1.scale.x;
//			g_rendermanager.camera.adjust_view(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			p1.pos.y -= p1.scale.y;
//			g_rendermanager.camera.adjust_view(0.0, -1.0, 0.0);
		}		
		if (player_info[1]->check_button_on(BUTTON_UP)) {
			p2.pos.y += p2.scale.y;
//			g_rendermanager.camera.add_pos(0.0, 0.0, 1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) {
			p2.pos.y -= p2.scale.y;
//			g_rendermanager.camera.add_pos(0.0, 0.0, -1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) {
			p2.pos.x += p2.scale.x;
//			g_rendermanager.camera.add_pos(1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) {
			p2.pos.x -= p2.scale.x;
//			g_rendermanager.camera.add_pos(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) {
			p1.rot.x += 0.1;
			p2.rot.x += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MP)) {
			p1.rot.y += 0.1;
			p2.rot.y += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) {
			p1.rot.z += 0.1;
			p2.rot.z += 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_LK)) {
			p1.rot.x -= 0.1;
			p2.rot.x -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_MK)) {
			p1.rot.y -= 0.1;
			p2.rot.y -= 0.1;
		}
		if (player_info[0]->check_button_on(BUTTON_HK)) {
			p1.rot.z -= 0.1;
			p2.rot.z -= 0.1;
		}
		
		for (int i = 0; i < 2; i++) {
			if (frame[i] > anim[i].length) {
				frame[i] = 0.0;
			}
			else {
				frame[i] += 0.2;
			}
		}

		p1.model.set_bones(frame[0], &anim[0]);
		p2.model.set_bones(frame[1], &anim[1]);

		p1.render();
		p2.render();

		p1_healthbar.render();
		p2_healthbar.render();

		SDL_GL_SwapWindow(g_window);
	}
	p1_healthbar.destroy();
	p2_healthbar.destroy();

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}