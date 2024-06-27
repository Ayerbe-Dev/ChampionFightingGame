#pragma once
#include "FighterMovelist.h"
#include "CircularBuffer.h"
#include "glm/glm.hpp"

struct CPUAction {
	InputKind input_kind;
	unsigned short required_buttons;
	unsigned int stick_dir;

	int startup;
	glm::vec2 range_x;
};

struct CPUFighterState {
	float facing_dir;
	glm::vec2 pos;
	glm::vec2 speed;
};

class FighterCPU {
public:
	FighterCPU();

	void init(Fighter* owner, Fighter* opponent);
	void add_action(std::string name, InputKind input_kind, unsigned short required_buttons,
		unsigned int stick_dir, int startup, glm::vec2 range_x);
	void process();

	Fighter* owner;
	Fighter* opponent;

	int reaction_time;

	std::map<std::string, CPUAction> actions;
	CircularBuffer<CPUFighterState> opponent_state;
private:
	void execute_action(CPUAction action);
	void determine_opponent_state();
	unsigned int numpad_to_bits(unsigned int numpad_dir);
};