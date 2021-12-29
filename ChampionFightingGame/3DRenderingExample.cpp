#include "3DRenderingExample.h"
#include "RenderManager.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include "utils.h"
#include "stb_image.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
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

	Camera camera;
	Shader shader("vertex_no_light.glsl", "fragment_no_light.glsl", &camera);
	Model backpack("resource/chara/roy/model/model.dae");

	vec3 pointLightPositions[] = {
	   vec3(0.7f,  0.2f,  2.0f),
	   vec3(2.3f, -3.3f, -4.0f),
	   vec3(-4.0f,  2.0f, -12.0f),
	   vec3(0.0f,  0.0f, -3.0f)
	};
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	shader.use();

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
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

		for (int i = 0; i < 2; i++) {
			if (player_info[i]->check_button_on(BUTTON_MENU_BACK)) {
				three_deeing = false;
			}
		}
		if (player_info[0]->check_button_on(BUTTON_UP)) { //Move the camera forward
			camera.add_pos(0.0, 0.0, -1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_RIGHT)) {
			camera.add_pos(1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LEFT)) {
			camera.add_pos(-1.0, 0.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_DOWN)) {
			camera.add_pos(0.0, 0.0, 1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LK)) {
			camera.add_pos(0.0, 1.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_HK)) {
			camera.add_pos(0.0, -1.0, 0.0);
		}
		if (player_info[0]->check_button_on(BUTTON_LP)) { //Zoom the camera in
			camera.adjust_view(0.0, 0.0, -1.0);
		}
		if (player_info[0]->check_button_on(BUTTON_HP)) { //Out
			camera.adjust_view(0.0, 0.0, 1.0);
		}
		if (player_info[1]->check_button_on(BUTTON_UP)) { //Rotate the camera up
			camera.adjust_view(0.0, 1.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_RIGHT)) { //Right
			camera.adjust_view(1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_LEFT)) { //Left
			camera.adjust_view(-1.0, 0.0, 0.0);
		}
		if (player_info[1]->check_button_on(BUTTON_DOWN)) { //Down
			camera.adjust_view(0.0, -1.0, 0.0);
		}

		shader.use();
		camera.update_view();
		mat4 view = camera.get_view();
		mat4 projection = perspective(radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);
		mat4 model = mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));

		shader.set_vec3("view_pos", camera.pos);
		shader.set_float("material.shininess", 64.0f);

		shader.set_mat4("projection", projection);
		shader.set_mat4("view", view);
		shader.set_mat4("model", model);

		shader.use_default_dir_light(true);
		shader.set_point_lights(vec3(0.7, 0.2, 2.0));
		shader.use_default_point_light(1);
		shader.use_default_spot_light(true);
		shader.update_active_lights();

		backpack.render(shader);

		SDL_GL_SwapWindow(g_window);
	}

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}