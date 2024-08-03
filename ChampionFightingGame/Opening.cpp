#include "Opening.h"
#include "GameTexture.h"
#include "WindowManager.h"
#include "TimeFuncs.h"

void opening_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	GameTexture titleSplash;
	titleSplash.init("resource/game_state/opening/game-splash-background.png");

	GameTexture textSplash;
	textSplash.init("resource/game_state/opening/game-splash-text.png");

	char title_alpha = 0;
	char text_alpha = 0;
	int fade_count = 0;
	int fade_state = 0;

	bool loop = true;

	while (loop && game_manager->next_game_state != GAME_STATE_CLOSE) {
		wait_ms(66.667);

		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_menu();
			if (player[i]->controller.is_any_inputs()) {
				loop = false;
			}
		}

		window_manager->clear_screen();

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
			if (fade_count == 23) {
				loop = false;
			}
		}

		titleSplash.set_alpha(title_alpha);
		textSplash.set_alpha(text_alpha);
		titleSplash.render();
		textSplash.render();

		window_manager->update_screen();
	}

	titleSplash.destroy();
	textSplash.destroy();
}