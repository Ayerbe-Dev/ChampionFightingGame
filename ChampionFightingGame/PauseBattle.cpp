#include "PauseBattle.h"
#include "RenderManager.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameMenu* background_menu = game_manager->get_target();

	game_manager->layer++;

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	const Uint8* keyboard_state;

	PauseBattle* pause = new PauseBattle;
	pause->load_game_menu();

	while (*pause->looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();
		
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		background_menu->process_background();
		pause->panel.render();

		SDL_GL_SwapWindow(render_manager->window);
	}

	delete pause;
	game_manager->layer--;
}

PauseBattle::PauseBattle() {

}

PauseBattle::~PauseBattle() {
	panel.destroy();
}

void PauseBattle::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();

	game_manager->set_menu_info(this);

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	panel.init("resource/game_state/menu/options/overlay.png");
	panel.set_width(500);
	panel.set_height(300);
	panel.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

void PauseBattle::event_start_press() {
	*looping = false;
}