#include "AIManager.h"

AIInfo::AIInfo() {

}

AIInfo::AIInfo(float x_pos, float y_pos, std::string anim_kind, float frame, float rate) {
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->anim_kind = anim_kind;
	this->frame = frame;
	this->rate = rate;
}

AIManager* AIManager::instance = nullptr;

AIManager::AIManager() {
	for (int i = 0; i < 2; i++) {
		ai_info[i].resize(20);
	}
}

AIManager* AIManager::get_instance() {
	if (instance == nullptr) {
		instance = new AIManager;
	}
	return instance;
}