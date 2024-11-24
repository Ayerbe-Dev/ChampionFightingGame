#include "Options.h"
#include "WindowManager.h"
#include "TimeFuncs.h"

void controls_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	GameState* background_menu = game_manager->get_game_state();

	OptionsMenu* options_menu = new OptionsMenu;

	while (options_menu->looping) {
		wait_ms();
		window_manager->clear_screen();

		options_menu->process_game_state();

		background_menu->process_main();
		background_menu->render_main();
		options_menu->panel.render();

		window_manager->update_screen();
	}

	delete options_menu;
}

OptionsMenu::OptionsMenu() {
	GameManager* game_manager = GameManager::get_instance();

	panel.init("resource/game_state/menu/options/overlay.png");
	panel.set_width(500);
	panel.set_height(300);
	panel.set_orientation(SCREEN_TEXTURE_ORIENTATION_MIDDLE_LEFT);
}

OptionsMenu::~OptionsMenu() {
	panel.destroy();
}

void OptionsMenu::event_back_press() {
	looping = false;
}