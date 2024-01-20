#include "PauseBattle.h"
#include "RenderManager.h"
#include "ObjectManager.h"
#include "Fighter.h"
#include "TimeFuncs.h"

void pause_battle_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	GameState* background_menu = game_manager->get_game_state();

	PauseBattle* pause = new PauseBattle;

	while (pause->looping) {
		game_manager->frame_delay_check_fps();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render_manager->handle_window_events();

		pause->process_game_state();
		pause->render_game_state();
		
		render_manager->update_screen();
	}

	delete pause;
}

PauseBattle::PauseBattle() {
	GameManager* game_manager = GameManager::get_instance();

	if (game_context == GAME_CONTEXT_TRAINING) {
		push_menu_object("Pause"); {
			push_menu_texture("Panel", "resource/game_state/menu/options/overlay.png");
			push_menu_left_event_function([this](MenuObject* object) {
				object->get_activity_group("Training Pages").inc_active_child(-1);
			});
			push_menu_right_event_function([this](MenuObject* object) {
				object->get_activity_group("Training Pages").inc_active_child(-1);
			});
			push_menu_activity_group("Training Pages", nullptr, true); {
				push_menu_child("Pause Page"); {
					push_menu_activity_group("Pause Page Buttons", nullptr, false); {

					} pop_menu_stack();
				} pop_menu_stack();
			} pop_menu_stack();
		} pop_menu_stack();
	}
	else {
		push_menu_object("Pause"); {
			push_menu_texture("Panel", "resource/game_state/menu/options/overlay.png");
		} pop_menu_stack();
	}
}

PauseBattle::~PauseBattle() {
	
}

void PauseBattle::process_main() {
	GameManager* game_manager = GameManager::get_instance();
	for (size_t i = 0, max = menu_objects.size(); i < max; i++) {
		menu_objects[i].event_process();
	}
}

void PauseBattle::render_main() {
	render_menu_object("Pause");
}

void PauseBattle::event_start_press() {
	looping = false;
}

void PauseBattle::event_select_press() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->update_state(GAME_STATE_DEBUG_MENU);
	game_manager->get_game_state(1)->looping = false;
}

void PauseBattle::event_back_press() {
	ObjectManager* object_manager = ObjectManager::get_instance();
	object_manager->fighter[0]->reset();
	object_manager->fighter[1]->reset();
	looping = false;
}

void PauseBattle::event_up_press() {
	get_menu_object("Pause").event_up_press();
}

void PauseBattle::event_down_press() {
	get_menu_object("Pause").event_down_press();
}

void PauseBattle::event_left_press() {
	get_menu_object("Pause").event_left_press();
}

void PauseBattle::event_right_press() {
	get_menu_object("Pause").event_right_press();
}