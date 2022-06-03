#include "GameMenu.h"
#include "Loader.h"
#include "GameManager.h"

void GameMenu::event_up_press(){}
void GameMenu::event_down_press(){}
void GameMenu::event_left_press(){}
void GameMenu::event_right_press(){}
void GameMenu::event_start_press(){}
void GameMenu::event_select_press(){}
void GameMenu::event_back_press(){}
void GameMenu::event_pause_press() {}
void GameMenu::event_any_press() {}
void GameMenu::process_background() {}

void GameMenu::load_game_menu() {
	GameManager::get_instance()->set_menu_info(this);
}

GameMenu::GameMenu() {
	average_ticks.reserve(10000);
	looping = nullptr;
	game_state = nullptr;
	prev_game_state = nullptr;
	game_context = nullptr;
	prev_game_context = nullptr;
	game_loader = nullptr;
}

void GameMenu::update_state(int game_state, int game_context) {
	if (game_state != GAME_STATE_MAX) {
		*prev_game_state = *this->game_state;
		*this->game_state = game_state;
	}
	if (game_context != GAME_CONTEXT_MAX) {
		*prev_game_context = *this->game_context;
		*this->game_context = game_context;
	}
}

void GameMenu::inc_thread() {
	update_thread_progress(game_loader->loaded_items);
}

void GameMenu::frame_delay() {
	wait_ms();
}

void GameMenu::frame_delay_check_performance() {
	int trials = 10000;

	if (average_ticks.size() < trials) {
		average_ticks.push_back((float)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ms).count())/1000.0);
	}
	else {
		float highest = average_ticks[0];
		while (highest >= tick_frequency.size()) {
			tick_frequency.push_back(0);
		}
		int freq = 0;
		int frame_freq = 0;
		float total = 0;
		for (int i = 0; i < trials; i++) {
			total += average_ticks[i];
			if (average_ticks[i] >= 16.667) {
				frame_freq++;
			}
			if (average_ticks[i] > highest) {
				highest = average_ticks[i];
				while (highest >= tick_frequency.size()) {
					tick_frequency.push_back(0);
				}
				freq = 1;
			}
			else if (average_ticks[i] == highest) {
				freq++;
			}
			tick_frequency[(int)average_ticks[i]]++;
		}
		total /= (float)trials;
		std::cout << "Lengths of all iterations across " << trials << " tests: " << "\n";
		for (int i = 0; i < tick_frequency.size(); i++) {
			std::cout << "MS: " << i << ", Frequency: " << tick_frequency[i] << "\n";
		}
		std::cout << "On average, it took " << total << " ms to run the loop, and there were " << frame_freq << " instances of an iteration taking more than a frame." << "\n";
		average_ticks.clear();
		tick_frequency.clear();
	}
	wait_ms();
	ms = std::chrono::high_resolution_clock::now();
}