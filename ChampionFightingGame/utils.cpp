#include "utils.h"
#include <sstream>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>

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

float clampf(float min, float value, float max) {
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

/// <summary>
/// Takes a string and removes a filtered version of it.
/// </summary>
/// <param name="to">: Base string</param>
/// <param name="remove">: Part of the string to remove</param>
/// <returns>Filtered string</returns>
std::string filter_string(const std::string& to, const std::string& remove) {
	if (remove == "") return to;
	std::string ret = "";
	std::string ret2 = "";
	int removal_index = to.find(remove);
	if (removal_index == -1) {
		return to;
	}
	int cont_index = removal_index + remove.length();
	ret = to.substr(0, removal_index);
	ret2 = to.substr(cont_index, to.length());

	return filter_string(ret + ret2, remove);
}

//Check the first character in a string that is a space
int get_blank(std::string s) {
	const char* c = s.c_str();
	for (int i = 0; i < s.length(); i++) {
		if (isblank(c[i])) {
			return i;
		}
	}
	return 0;
}

float get_relative_one_percent(float val, float denom) {
	float mul = denom / 100.0;
	return (val / 100.0) * mul;
}

int round_up_odd(int val) {
	return (val / 2) + (val % 2);
}

int rng(const int& min, const int& max) {
	thread_local std::mt19937* generator = nullptr;
	std::hash<std::thread::id> hasher;
	if (!generator) generator = new std::mt19937(clock() + hasher(std::this_thread::get_id()));
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*generator);
}

float rng_f(const float& min, const float& max) {
	thread_local std::mt19937* generator = nullptr;
	std::hash<std::thread::id> hasher;
	if (!generator) generator = new std::mt19937(clock() + hasher(std::this_thread::get_id()));
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(*generator);
}

float lerp(float a, float b, float f) {
	return a + f * (b - a);
}

bool is_between(float a, float mid, float b) {
	return (a >= mid && b <= mid) || (a <= mid && b >= mid);
}

std::int32_t convert_to_int(char* buffer, std::size_t len) {
	std::int32_t a = 0;
	if (std::endian::native == std::endian::little) {
		std::memcpy(&a, buffer, len);
	}
	else {
		for (std::size_t i = 0; i < len; ++i) {
			reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
		}
	}
	return a;
}

std::string float_to_string(float val, int precision) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << val;
	std::string ret = ss.str();
	for (size_t i = ret.size() - 1; ret[i] == '0' && ret[i-1] != '.'; i--) {
		ret.pop_back();
	}
	return ret;
}