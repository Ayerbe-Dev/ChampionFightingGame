#pragma once
#include <iostream>
class EffectInfo {
public:
	EffectInfo();

	std::string name;
	std::string dir;
	int duration;
};

class Effect {
public:
	Effect();
	EffectInfo info;
};