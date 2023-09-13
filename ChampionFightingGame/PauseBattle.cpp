#include "PauseBattle.h"
#include "RenderManager.h"
#include "BattleObjectManager.h"
#include "Fighter.h"
#include "utils.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameState* background_menu = game_manager->get_target();

	game_manager->layer++;

	Player* player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	PauseBattle* pause = new PauseBattle;

	while (*pause->looping) {
		wait_ms();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();
		
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
			player[i]->controller.poll_buttons();
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
	GameManager* game_manager = GameManager::get_instance();

	game_manager->set_menu_info(this);

	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

	panel.init("resource/game_state/menu/options/overlay.png");
	panel.set_width(500);
	panel.set_height(300);
	panel.set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

PauseBattle::~PauseBattle() {
	panel.destroy();
}

void PauseBattle::event_start_press() {
	*looping = false;
}

void PauseBattle::event_select_press() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->update_state(GAME_STATE_DEBUG_MENU);
	game_manager->looping[game_manager->layer - 1] = false;
}

void PauseBattle::event_back_press() {
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	battle_object_manager->fighter[0]->reset();
	battle_object_manager->fighter[1]->reset();
	*looping = false;
}