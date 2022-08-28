#include "Opening.h"
#include <glew/glew.h>
#include "GameTexture.h"
#include "RenderManager.h"

void opening_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	const Uint8* keyboard_state;
	GameTexture titleSplash;
	titleSplash.init("resource/game_state/opening/game-splash-background.png");

	GameTexture textSplash;
	textSplash.init("resource/game_state/opening/game-splash-text.png");

	char title_alpha = 0;
	char text_alpha = 0;
	int fade_count = 0;
	int fade_state = 0;

	game_manager->looping[game_manager->layer] = true;

	while (game_manager->looping[game_manager->layer]) {
		wait_ms(66.667);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_buttons(keyboard_state);
			if (player[i]->controller.is_any_inputs()) {
				game_manager->looping[game_manager->layer] = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (fade_state == 0) {
			title_alpha += 10;
			fade_count++;
			if (fade_count == 24) {
				fade_state++;
				fade_count = 0;
			}
		}
		else if (fade_state == 1) {
			text_alpha += 5;
			fade_count++;
			if (fade_count == 50) {
				fade_state++;
				fade_count = 0;
			}
		}
		else {
			title_alpha -= 10;
			text_alpha -= 10;
			fade_count++;
			if (fade_count == 24) {
				game_manager->looping[game_manager->layer] = false;
			}
		}

		titleSplash.set_alpha(title_alpha);
		textSplash.set_alpha(text_alpha);
		titleSplash.render();
		textSplash.render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	titleSplash.destroy();
	textSplash.destroy();
}