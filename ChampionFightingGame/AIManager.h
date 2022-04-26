#pragma once
#include <vector>

struct AIInfo {
	float x_pos;
	float y_pos;
	int status_kind;
};

class AIManager {
public:
	AIManager(AIManager& other) = delete;
	void operator=(const AIManager& other) = delete;

	std::vector<AIInfo> ai_info[2]; //todo: make a circular buffer class and replace the vector w that

	static AIManager* get_instance();

private:
	AIManager();
	static AIManager* instance;
};