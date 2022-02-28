#include "GameMenu.h"
#include "Loader.h"

void GameMenu::event_up_press(){};
void GameMenu::event_down_press(){};
void GameMenu::event_left_press(){};
void GameMenu::event_right_press(){};
void GameMenu::event_start_press(){};
void GameMenu::event_select_press(){};
void GameMenu::event_back_press(){};
void GameMenu::event_any_press() {};

void GameMenu::process_background() {};

GameMenu::GameMenu() {

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
	frameTimeDelay();
}

void GameMenu::frame_delay_check_performance() {
	int trials = 1000;

	if (average_ticks.size() < trials) {
		average_ticks.push_back(SDL_GetTicks() - ticks);
	}
	else {
		int highest = average_ticks[0];
		while (highest >= tick_frequency.size()) {
			tick_frequency.push_back(0);
		}
		int freq = 0;
		int frame_freq = 0;
		float total = 0;
		for (int i = 0; i < trials; i++) {
			total += (float)average_ticks[i];
			if (average_ticks[i] >= 16) {
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
			tick_frequency[average_ticks[i]]++;
		}
		total /= (float)trials;
		cout << "Lengths of all iterations across " << trials << " tests: " << endl;
		for (int i = 0; i < tick_frequency.size(); i++) {
			cout << "MS: " << i << ", Frequency: " << tick_frequency[i] << endl;
		}
		cout << "On average, it took " << total << " ms to run the loop, and there were " << frame_freq << " instances of an iteration taking more than a frame." << endl;
		average_ticks.clear();
		tick_frequency.clear();
	}
	frameTimeDelay();
	ticks = SDL_GetTicks();
}