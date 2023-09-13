#include "AIManager.h"

AIInfo::AIInfo() {
	frame = 0.0;
	rate = 0.0;
	x_pos = 0.0;
	y_pos = 0.0;
}

AIInfo::AIInfo(float x_pos, float y_pos, std::string anim_kind, float frame, float rate) {
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->anim_kind = anim_kind;
	this->frame = frame;
	this->rate = rate;
}

AIManager::AIManager() {
	for (int i = 0; i < 2; i++) {
		ai_info[i].resize(20);
	}
}

AIManager* AIManager::instance = nullptr;
AIManager* AIManager::get_instance() {
	if (instance == nullptr) {
		instance = new AIManager;
	}
	return instance;
}

void AIManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}