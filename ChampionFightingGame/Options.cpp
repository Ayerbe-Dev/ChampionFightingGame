#include "Options.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "TimeFuncs.h"

void controls_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	Scene* background_menu = game_manager->get_scene();

	OptionsMenu* options_menu = new OptionsMenu;

	while (options_menu->looping) {
		wait_ms();
		window_manager->clear_screen();

		options_menu->process();
		background_menu->process_main();
		options_menu->render();

		window_manager->update_screen();
	}

	delete options_menu;
}

OptionsMenu::OptionsMenu() {
	GameManager* game_manager = GameManager::get_instance();
	root.add_element("Panel", ScreenTexture("resource/scene/menu/options/overlay.png")
		.set_base_width(500)
		.set_base_height(300)
		.set_orientation(TEXTURE_LEFT)
	).add_event("back_press", [this](SceneElement* elem) {
		looping = false;
	});
}

OptionsMenu::~OptionsMenu() {
}

void OptionsMenu::render_main() {
	root.render();
}