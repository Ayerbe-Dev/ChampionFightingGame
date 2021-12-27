#include "3DRenderingExample.h"
#include "RenderManager.h"
#include <glm.hpp>
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

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

	Vertex vertices[] = {
		glm::vec3(0.0, 0.5, 0.0),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.0, 1.0),
		glm::vec3(-0.5, -0.5, 0.0),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.0, 0.0),
		glm::vec3(0.5, -0.5, 0.0),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.0, 0.0)
	};

	unsigned vertCount = sizeof(vertices) / sizeof(Vertex);

	GLuint indices[] = { 
		0, 1, 2
	};
	unsigned indexCount = sizeof(indices) / sizeof(GLuint);

	u32 core_program = 0;
	load_shaders(&core_program);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, col));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	while (three_deeing) {
		frameTimeDelay();
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(core_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

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

		SDL_GL_SwapWindow(g_window);
	}

	glDeleteProgram(core_program);
	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}