#include "TitleScreen.h"
#include "GameManager.h"
#include "FontManager.h"
#include "WindowManager.h"
#include "cotr_imgui_debugger.h"
#include "TimeFuncs.h"
#include <glew/glew.h>

void title_screen_main() {
	GameManager* game_manager = GameManager::get_instance();
	WindowManager* window_manager = WindowManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	font_manager->load_face("Fiend-Oblique");
	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];

    TitleScreen *title_screen = new TitleScreen;

    while (title_screen->looping) {
		wait_ms();

		window_manager->clear_screen();

		title_screen->process();
		title_screen->render();

		window_manager->update_screen();
	}
	delete title_screen;
	font_manager->unload_face("Fiend-Oblique");
}

TitleScreen::TitleScreen() {
	GameManager* game_manager = GameManager::get_instance();
	load_font("Title Font", "Fiend-Oblique", 64);
	root.add_elements({
		{"TitleL1", ScreenTexture("resource/scene/title/ui/title-l4.png")},
		{"TitleL2", ScreenTexture("resource/scene/title/ui/title-l3.png")},
		{"TitleL3", ScreenTexture("resource/scene/title/ui/title-l2.png")},
		{"TitleL4", ScreenTexture("resource/scene/title/ui/title-l1.png")},
		{"Title", ScreenText(&get_font("Title Font"), "Champions of the Ring", TextSpecifier().border(4))}
	}).add_event("button_press", [this](SceneElement* elem) {
		update_scene(SCENE_MAIN_MENU, SCENE_CONTEXT_NONE);
	});
}

TitleScreen::~TitleScreen() {

}

void TitleScreen::render_main() {
	root.render();
}