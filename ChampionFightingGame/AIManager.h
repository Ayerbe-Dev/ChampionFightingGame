#pragma once
#include "CircularBuffer.h"
#include <mutex>

struct AIInfo {
	AIInfo();
	AIInfo(float x_pos, float y_pos, std::string anim_kind, float frame, float rate);
	float x_pos;
	float y_pos;
	std::string anim_kind;
	float frame;
	float rate;
};

class AIManager {
public:
	AIManager(AIManager& other) = delete;
	void operator=(const AIManager& other) = delete;

	CircularBuffer<AIInfo> ai_info[2];
	std::mutex ai_mutex;
	static AIManager* get_instance();

private:
	AIManager();
	static AIManager* instance;
};