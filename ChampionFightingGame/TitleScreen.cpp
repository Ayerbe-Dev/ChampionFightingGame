#include "TitleScreen.h"
#include "GameTexture.h"
#include "Debugger.h"
#include <glew/glew.h>
#include "RenderManager.h"

void title_screen_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();

	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];

    TitleScreen *title_screen = new TitleScreen;

    const Uint8 *keyboard_state;

	game_manager->set_menu_info(title_screen);

    while (game_manager->looping[game_manager->layer]) {
		wait_ms();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(nullptr);
		for (int i = 0; i < 2; i++) {
			player_info[i]->controller.check_controllers();
			player_info[i]->controller.poll_buttons(keyboard_state);
		}

		game_manager->handle_menus();

		title_screen->render();

		SDL_GL_SwapWindow(render_manager->window);
	}


	delete title_screen;
	return game_manager->update_state(GAME_STATE_MENU);
}

TitleScreen::TitleScreen() {
    title_l1.init("resource/ui/title/title-l1.png");
    title_l2.init("resource/ui/title/title-l2.png");
    title_l3.init("resource/ui/title/title-l3.png");
    title_l4.init("resource/ui/title/title-l4.png");
    text.init("resource/ui/title/Praeiudicium.png");
}

TitleScreen::~TitleScreen() {
	title_l1.destroy();
	title_l2.destroy();
	title_l3.destroy();
	title_l4.destroy();
	text.destroy();
}

void TitleScreen::render(){
	title_l4.render();
	title_l3.render();
	title_l2.render();
	title_l1.render();
	text.render();
}

void TitleScreen::event_any_press(){
	*looping = false;
}