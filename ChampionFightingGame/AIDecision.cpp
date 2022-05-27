#include "AI.h"
#include "utils.h"

void AI::decision_main() {
	if (rng(0, 10) >= stubbornness || active_choice == nullptr) {
		for (int i = 0, max = move_list.size(); i < max; i++) {
			//Figure out which moves in our move list are "optimal"
			if (is_in_range(move_list[i])) {
				if ((fast_enough(move_list[i]) || they_wont_press_here(move_list[i]))
					&& (is_safe(move_list[i]) || will_hit(move_list[i]))
					|| fuck_it(move_list[i])) { //If it passes all of the checks or the AI says fuck it,
					//it will consider using that move
					chosen_actions.push_back(&move_list[i]);
				}
			}
		}

		AIMoveList* old_choice = active_choice;

		int rng_index = rng(0, chosen_actions.size() - 1); //Randomly pick one of our "optimal" moves to
		//try executing

		active_choice = chosen_actions[rng_index];

		if (active_choice != old_choice) { //If we changed our choice, reset the input stage. If not, leave it.
			input_stage = 0;
		}
	
		//The following warning is factually incorrect.

		if (active_choice->motion_input) { //If we decided on a motion input, we can't start thinking
		//about what to do next until we finish the input.
			committed = true;
		}
	}
}

bool AI::is_in_range(AIMoveList& move) {
	return true;
}

bool AI::fast_enough(AIMoveList& move) {
	return true;
}

bool AI::they_wont_press_here(AIMoveList& move) {
	return true;
}

bool AI::is_safe(AIMoveList& move) {
	return true;
}

bool AI::will_hit(AIMoveList& move) {
	return true;
}

bool AI::fuck_it(AIMoveList& move) {
	int impulse = rng(0, 10);
	return impulse > judgement;
}


bool AI::can_use(AIMoveList& move) {
	return true;
}