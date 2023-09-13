#pragma once
#include <iostream>
#include <vector>
#include "GameTexture.h"


class TextUI {
public:
	TextUI();
private:
	bool freeze_duration;
	int duration;
	int fadeout_time;
	std::string text;

};