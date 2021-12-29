#include "3DRenderingExample.h"
#include "RenderManager.h"
#include "utils.h"

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

	//Get rid of our SDL stuff for now

	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);

	Shader color_shader("vertex_mat.glsl", "fragment_mat.glsl");
	Shader light_source("vertex_light_source.glsl", "fragment_light_source.glsl");
	Camera camera;

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//Creating multiple cubes, these are just offsets for each one.
	vec3 cubePositions[] = {
	vec3(0.0f,  0.0f,  0.0f),
	vec3(2.0f,  5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f,  2.0f, -2.5f),
	vec3(1.5f,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
	};
	vec3 pointLightPositions[] = {
	   vec3(0.7f,  0.2f,  2.0f),
	   vec3(2.3f, -3.3f, -4.0f),
	   vec3(-4.0f,  2.0f, -12.0f),
	   vec3(0.0f,  0.0f, -3.0f)
	};
	vec3 lightPos(1.2f, 1.0f, 2.0f);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);	

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(lightCubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	unsigned int light_texture1 = loadGLTexture("resource/chara/roy/sprite/container2.png");
	unsigned int light_texture2 = loadGLTexture("resource/chara/roy/sprite/container2_specular.png");

	color_shader.use();
	color_shader.set_int("material.diffuse", 0);
	color_shader.set_int("material.specular", 1);

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

		camera.update_view();
		mat4 view = camera.get_view();
		mat4 projection = perspective(radians(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f);
		mat4 model = mat4(1.0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		color_shader.use();
		color_shader.set_vec3("view_pos", camera.pos);
		color_shader.set_float("material.shininess", 32.0f);

		color_shader.set_vec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
		color_shader.set_vec3("dir_light.ambient", 0.05f, 0.05f, 0.05f);
		color_shader.set_vec3("dir_light.diffuse", 0.4f, 0.4f, 0.4f);
		color_shader.set_vec3("dir_light.specular", 0.5f, 0.5f, 0.5f);
		for (int i = 0; i < 4; i++) {
			string point_lights = ("point_lights[" + to_string(i) + "].");
			color_shader.set_vec3((point_lights + "position").c_str(), pointLightPositions[i]);
			color_shader.set_vec3((point_lights + "ambient").c_str(), 0.05, 0.05, 0.05);
			color_shader.set_vec3((point_lights + "diffuse").c_str(), 0.8, 0.8, 0.8);
			color_shader.set_vec3((point_lights + "specular").c_str(), 1.0, 1.0, 1.0);
			color_shader.set_float((point_lights + "constant").c_str(), 1.0);
			color_shader.set_float((point_lights + "constant").c_str(), 1.0);
			color_shader.set_float((point_lights + "linear").c_str(), 0.09);
			color_shader.set_float((point_lights + "quadratic").c_str(), 0.032);
		}
		color_shader.set_vec3("spot_light.position", camera.pos);
		color_shader.set_vec3("spot_light.direction", camera.front);
		color_shader.set_vec3("spot_light.ambient", 0.0f, 0.0f, 0.0f);
		color_shader.set_vec3("spot_light.diffuse", 1.0f, 1.0f, 1.0f);
		color_shader.set_vec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
		color_shader.set_float("spot_light.constant", 1.0f);
		color_shader.set_float("spot_light.linear", 0.09);
		color_shader.set_float("spot_light.quadratic", 0.032);
		color_shader.set_float("spot_light.cutoff", cos(radians(12.5f)));
		color_shader.set_float("spot_light.outer_cutoff", cos(radians(15.0f)));

		color_shader.set_mat4("view", view);
		color_shader.set_mat4("projection", projection);
		color_shader.set_mat4("model", model);

		light_source.use();
		light_source.set_mat4("view", view);
		light_source.set_mat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, light_texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, light_texture2);

		color_shader.use();
		glBindVertexArray(cubeVAO);
		for (int i = 0; i < 10; i++) {
			mat4 model = mat4(1.0f);
			model = translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = rotate(model, radians(angle), vec3(1.0f, 0.3f, 0.5f));
			color_shader.set_mat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		light_source.use();
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			light_source.set_mat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		SDL_GL_SwapWindow(g_window);
	}
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);

	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}