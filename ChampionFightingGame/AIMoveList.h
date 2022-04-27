#pragma once
#include <vector>
#include <iostream>

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
	std::vector<AIMoveList*> cancel_followups;
	int meter_cost;
	int charge_frames;
};