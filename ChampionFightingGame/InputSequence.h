#pragma once
#include "Button.h"
#include <vector>

struct InputCode {
	InputCode();
	InputCode(unsigned short code);
	InputCode(unsigned short code, unsigned short duration);

	unsigned short code;
	unsigned short duration;
};

class InputSequence {
public:
	InputSequence();
	
	void add_inputs(unsigned short input_code);
	void set_inputs(InputCode input_code);
	void set_input_seq(std::vector<InputCode> input_seq);
	unsigned short get_curr_input_code();
	unsigned short get_curr_input_code_cpu();
	unsigned short get_prev_input_code(int offset);
	void reset_idx();
private:
	std::size_t idx;
	unsigned short input_seq[8192];
};