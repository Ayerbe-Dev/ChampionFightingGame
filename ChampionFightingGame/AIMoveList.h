#pragma once
#include <vector>
#include <iostream>

enum {
	AI_INPUT_NONE,
	AI_INPUT_FORWARD,
	AI_INPUT_BACK,
	AI_INPUT_UP,
	AI_INPUT_DOWN,
	AI_INPUT_LP,
	AI_INPUT_MP,
	AI_INPUT_HP,
	AI_INPUT_LK,
	AI_INPUT_MK,
	AI_INPUT_HK,
};

struct AIMoveList {
	std::string name;
	float range_x;
	float range_y;
	bool hits_ground;
	bool hits_air;
	bool hits_otg;
	int startup;
	float damage;
	float chip_damage;
	int hit_advantage;
	int block_advantage;
	bool commitment;
	bool motion_input;
	std::vector<AIMoveList*> cancel_followups;
	std::vector<std::vector<unsigned int>> inputs;
	int meter_cost;
	int charge_frames;
};