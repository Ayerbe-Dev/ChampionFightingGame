#include "PauseBattle.h"
#include "RenderManager.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameMenu* background_menu = game_manager->get_target();

	game_manager->layer++;

	PlayerInfo* player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

	const Uint8* keyboard_state;

	PauseBattle pause(500, 300, "resource/ui/battle/pause/overlay.png");
	pause.player_info[0] = player_info[0];
	pause.player_info[1] = player_info[1];

	game_manager->set_menu_info(&pause);

	while (*pause.looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();
		
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
			player_info[i]->controller.poll_buttons(keyboard_state);
			if (player_info[i]->controller.check_button_trigger(BUTTON_START)) {
				*pause.looping = false;
			}
		}

		background_menu->process_background();
		pause.panel.render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	pause.panel.destroy();
	game_manager->layer--;
}

PauseBattle::PauseBattle() {

}

PauseBattle::PauseBattle(int width, int height, std::string dir) {
	init(width, height, dir);
}

void PauseBattle::init(int width, int height, std::string dir) {
	panel.init(dir);
	panel.set_width(width);
	panel.set_height(height);
	panel.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}