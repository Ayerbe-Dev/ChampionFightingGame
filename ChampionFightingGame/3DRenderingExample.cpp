#include "3DRenderingExample.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void three_d_rendering_main(GameManager* game_manager) {
	PlayerInfo* player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	bool three_deeing = true;

	SDL_RenderClear(g_renderer);
	SDL_RenderPresent(g_renderer);
	
	while (three_deeing) {
		frameTimeDelay();

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

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		SDL_GL_SwapWindow(g_window);
	}
	game_manager->update_state(GAME_STATE_DEBUG_MENU);
}