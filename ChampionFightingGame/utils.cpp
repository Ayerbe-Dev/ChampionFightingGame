using namespace std;
#include <iostream>
#include <sstream>
#include "utils.h"
int clamp(int min, int value, int max) {
	if (min <= max) {
		if (value < min) {
			value = min;
		}
		if (value > max) {
			value = max;
		}
	}
	return value;
}

int ymlChopInt(string line) {
	return stoi(line.substr(line.find("=") + 1));
}

string ymlChopString(string line) {
	return line.substr(line.find("=") + 1);
}