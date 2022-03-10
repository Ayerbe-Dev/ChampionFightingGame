#pragma once
#include "SettingConstants.h"
#include <iostream>

struct Setting {
	std::string name = "";
	int val = 0;
};

class GameSettings {
public:
	GameSettings();
	Setting settings[SETTING_KIND_MAX];
};

int getGameSetting(std::string setting);
void setGameSetting(std::string setting, int val);
void updateGameSettings();
