#include "InputSequence.h"

InputSequence::InputSequence() {
	idx = 0;
	input_seq[1] = USHRT_MAX;
}

void InputSequence::add_inputs(unsigned short input_code) {
	input_seq[idx++] = input_code;
	if (idx == 8192) {
		idx = 0;
	}
	input_seq[idx] = USHRT_MAX;
}

void InputSequence::set_inputs(InputCode input_code) {
	for (idx = 0; idx < input_code.duration; idx++) {
		input_seq[idx] = input_code.code;
	}
	input_seq[idx] = USHRT_MAX;
	idx = 0;
}

void InputSequence::set_input_seq(std::vector<InputCode> input_seq) {
	idx = 0;
	for (std::size_t i = 0, max = input_seq.size(); i < max; i++) {
		for (; idx < input_seq[i].duration; idx++) {
			this->input_seq[idx] = input_seq[i].code;
		}
	}
	input_seq[idx] = USHRT_MAX;
	idx = 0;
}

unsigned short InputSequence::get_curr_input_code() {
	if (idx == 8192 || input_seq[idx] == USHRT_MAX) {
		idx = 0;
	}
	return input_seq[idx++];
}

unsigned short InputSequence::get_prev_input_code(int offset) {
	if (offset > idx) {
		return input_seq[0];
	}
	return input_seq[idx - offset];
}

void InputSequence::reset_idx() {
	idx = 0;
}

InputCode::InputCode() {
	this->code = USHRT_MAX;
	this->duration = 1;
}

InputCode::InputCode(unsigned short code) {
	this->code = code;
	this->duration = 1;
}

InputCode::InputCode(unsigned short code, unsigned short duration) {
	this->code = code;
	this->duration = duration;
}