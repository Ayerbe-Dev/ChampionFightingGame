#pragma once
#include "utils.h"

struct Setting {
	string name = "";
	int val = 0;
};

class GameSettings {
public:
	GameSettings();
	Setting settings[SETTING_KIND_MAX];
};