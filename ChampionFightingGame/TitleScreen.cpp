#include "TitleScreen.h"
#include "GameTexture.h"
#include "cotr_imgui_debugger.h"
#include <glew/glew.h>
#include "WindowManager.h"
#include "TimeFuncs.h"

void title_screen_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();

	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

    TitleScreen *title_screen = new TitleScreen;

    while (title_screen->looping) {
		wait_ms();

		window_manager->clear_screen();

		title_screen->process_game_state();
		title_screen->render_game_state();

		window_manager->update_screen();
	}

	delete title_screen;
}

TitleScreen::TitleScreen() {
	GameManager* game_manager = GameManager::get_instance();

	title_l1.init("resource/game_state/title/ui/title-l1.png");
	title_l2.init("resource/game_state/title/ui/title-l2.png");
	title_l3.init("resource/game_state/title/ui/title-l3.png");
	title_l4.init("resource/game_state/title/ui/title-l4.png");
	text.init("resource/game_state/title/ui/Praeiudicium.png");
}

TitleScreen::~TitleScreen() {
	title_l1.destroy();
	title_l2.destroy();
	title_l3.destroy();
	title_l4.destroy();
	text.destroy();
}

void TitleScreen::render_main() {
	title_l4.render();
	title_l3.render();
	title_l2.render();
	title_l1.render();
	text.render();
}

void TitleScreen::event_any_press() {
	update_state(GAME_STATE_MAIN_MENU);
}