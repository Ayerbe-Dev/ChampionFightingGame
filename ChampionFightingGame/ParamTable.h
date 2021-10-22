using namespace std;
#include <iostream>
#include <sstream>
#include "utils.h"
#pragma once

struct Param {
	string stat{ "" };
	int type{-1};
	int value_i;
	float value_f;
	string value_s;
	bool value_b;
};