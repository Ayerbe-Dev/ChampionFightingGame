#include "FighterCPU.h"
#include "Fighter.h"

FighterCPU::FighterCPU() {
	owner = nullptr;
	opponent = nullptr;
	reaction_time = 16;
}

void FighterCPU::init(Fighter* owner, Fighter* opponent) {
	this->owner = owner;
	this->opponent = opponent;
	opponent_state.resize(30);
}

void FighterCPU::add_action(std::string name, InputKind input_kind, 
	unsigned short required_buttons, unsigned int stick_dir, 
	int startup, glm::vec2 range_x) {
	actions[name].input_kind = input_kind;
	actions[name].required_buttons = required_buttons;
	actions[name].stick_dir = stick_dir;
	actions[name].startup = startup;
	actions[name].range_x = range_x;
}

void FighterCPU::process() {
	CPUFighterState reaction_state = opponent_state.newest(reaction_time);
	bool executed_action = false;
	for (const auto &a : actions) {
		CPUAction action = a.second;
		glm::vec2 range_x = glm::vec2(
			owner->get_pos().x + action.range_x[0] * owner->facing_dir,
			owner->get_pos().x + action.range_x[1] * owner->facing_dir
		);
		glm::vec2 hypothetical_pos = (reaction_state.pos + reaction_state.speed) 
			* glm::vec2(action.startup);
		if (range_x[0] >= hypothetical_pos.x && range_x[1] <= hypothetical_pos.x
		|| range_x[0] <= hypothetical_pos.x && range_x[1] >= hypothetical_pos.x) {
			execute_action(action);
			executed_action = true;
		}
	}
	if (!executed_action) {
		owner->player->manual_seq.set_inputs(0);
	}

	determine_opponent_state();
}

void FighterCPU::execute_action(CPUAction action) {
	switch (action.input_kind) {
		case (INPUT_KIND_NORMAL): {
			owner->player->manual_seq.set_inputs(numpad_to_bits(action.stick_dir) | action.required_buttons);
		} break;
		default: {

		} break;
	}
}

void FighterCPU::determine_opponent_state() {
	CPUFighterState new_state;
	new_state.facing_dir = opponent->facing_dir;
	new_state.pos = glm::vec2(opponent->get_pos());
	new_state.speed = glm::vec2(opponent->get_pos()) - opponent_state.newest().pos;
	opponent_state.insert(new_state);
}

unsigned int FighterCPU::numpad_to_bits(unsigned int numpad_dir) {
	if (owner->facing_right) {
		switch (numpad_dir) {
			case (1): {
				return BUTTON_DOWN_BIT | BUTTON_LEFT_BIT;
			} break;
			case (2): {
				return BUTTON_DOWN_BIT;
			} break;
			case (3): {
				return BUTTON_DOWN_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (4): {
				return BUTTON_LEFT_BIT;
			} break;
			case (5): {
				return 0;
			} break;
			case (6): {
				return BUTTON_RIGHT_BIT;
			} break;
			case (7): {
				return BUTTON_UP_BIT | BUTTON_LEFT_BIT;
			} break;
			case (8): {
				return BUTTON_UP_BIT;
			} break;
			case (9): {
				return BUTTON_UP_BIT | BUTTON_RIGHT_BIT;
			} break;
		}
	}
	else {
		switch (numpad_dir) {
			case (1): {
				return BUTTON_DOWN_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (2): {
				return BUTTON_DOWN_BIT;
			} break;
			case (3): {
				return BUTTON_DOWN_BIT | BUTTON_LEFT_BIT;
			} break;
			case (4): {
				return BUTTON_RIGHT_BIT;
			} break;
			case (5): {
				return 0;
			} break;
			case (6): {
				return BUTTON_LEFT_BIT;
			} break;
			case (7): {
				return BUTTON_UP_BIT | BUTTON_RIGHT_BIT;
			} break;
			case (8): {
				return BUTTON_UP_BIT;
			} break;
			case (9): {
				return BUTTON_UP_BIT | BUTTON_LEFT_BIT;
			} break;
		}
	}
}