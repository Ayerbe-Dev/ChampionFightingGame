#include "AI.h"
#include "Fighter.h"

void AI::input_main() {
	if (input_stage == active_choice->inputs.size()) {
		active_choice = nullptr;
		committed = false;
		input_stage = 0;
		return;
	}

	if (active_choice->charge_frames != 0) {
		//Figure out which type of charge we need, and don't increment the input stage until we have it
		bool back_charge = false;
		bool down_charge = false;
		for (int i = 0, max = active_choice->inputs[input_stage].size(); i < max; i++) {
			if (active_choice->inputs[input_stage][i] == AI_INPUT_BACK) {
				back_charge = true;
			}
			else if (active_choice->inputs[input_stage][i] == AI_INPUT_DOWN) {
				down_charge = true;
			}
			set_input(active_choice->inputs[input_stage][i]);
		}
		if ((!down_charge || fighter->fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] >= active_choice->charge_frames)
			&& (!back_charge || fighter->fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] >= active_choice->charge_frames)) {
			input_stage++;
		}	
	}
	else {
		for (int i = 0, max = active_choice->inputs[input_stage].size(); i < max; i++) {
			set_input(active_choice->inputs[input_stage][i]);
		}
		input_stage++;
	}
}

void AI::set_input(unsigned int input) {
	switch (input) {
		case (AI_INPUT_NONE): {
			for (int i = 0; i < 4; i++) {
				controller->set_button_off(i);
			}
		} break;
		case (AI_INPUT_FORWARD): {
			if (fighter->internal_facing_right) {
				controller->set_button_on(BUTTON_RIGHT);
			}
			else {
				controller->set_button_on(BUTTON_LEFT);
			}
		} break;
		case (AI_INPUT_BACK): {
			if (fighter->internal_facing_right) {
				controller->set_button_on(BUTTON_LEFT);
			}
			else {
				controller->set_button_on(BUTTON_RIGHT);
			}
		} break;
		case (AI_INPUT_DOWN):
		case (AI_INPUT_UP): {
			controller->set_button_on(input);
		} break;
		default: {
			controller->set_button_on(input + 3);
		} break;
	}
}