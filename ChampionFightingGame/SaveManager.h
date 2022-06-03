#pragma once
#include <vector>
#include "PlayerInfo.h"

class SaveManager {
public:
	SaveManager(SaveManager& other) = delete;
	void operator=(const SaveManager& other) = delete;

	std::vector<PlayerInfo> player_info;

	static SaveManager* get_instance();
private:
	SaveManager();

	static SaveManager* instance;
};