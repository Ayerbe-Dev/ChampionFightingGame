#include "utils.h"
#include "SDL/SDL.h"
#include <sstream>
#include <iostream>
#include <chrono>

extern SDL_Renderer* g_renderer;
extern std::chrono::steady_clock::time_point g_chron;
extern SDL_mutex* file_mutex;
extern bool threadRender;

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

int ymlChopInt(std::string line) {
	return stoi(line.substr(line.find("=") + 1));
}

float ymlChopFloat(std::string line) {
	return stof(line.substr(line.find("=") + 1));
}

std::string ymlChopString(std::string line) {
	return line.substr(line.find("=") + 1);
}

/// <summary>
/// Takes a string and removes a filtered version of it.
/// </summary>
/// <param name="to">: Base string</param>
/// <param name="remove">: Part of the string to remove</param>
/// <returns>Filtered string</returns>
std::string Filter(const std::string& to, const std::string& remove) {
	std::string ret = "";
	std::string ret2 = "";
	int removal_index = to.find(remove);
	if (removal_index == -1) {
		return to;
	}
	int cont_index = removal_index + remove.length();
	ret = to.substr(0, removal_index);
	ret2 = to.substr(cont_index, to.length());

	return Filter(ret + ret2, remove);
}

/// <summary>
/// Pauses the current thread until 1 frame since the last time this function was called. Also called by loadSDLTexture in order to pause the loading
/// thread long enough for the main thread to consistently render.
/// </summary>
void wait_ms(double ms_duration) {
	using namespace std::chrono;
	steady_clock::time_point current_time = steady_clock::now();
	time_point<steady_clock, duration<double, std::nano>> future_time = steady_clock::now() + duration<double, std::milli>(ms_duration);

	//reduce the future time to account for processing time
	future_time -= (current_time - g_chron);

	while (current_time < future_time) {
		current_time = steady_clock::now();
	}
	g_chron = steady_clock::now();
};

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

/*
mankind knew that they cannot change find_nearest_css_slot. So instead of reflecting on themselves, they blamed std
\param heaven
\param or
\param hell
\param LET'S_ROCK!!
*/
int twoPointDistance(int x0, int y0, int x1, int y1) {
	return std::sqrt(std::pow(x0 - x1, 2) + std::pow(y0 - y1, 2));
}

float get_relative_one_percent(float val, float denom) {
	float mul = denom / 100.0;
	return (val / 100.0) * mul;
}

int round_up_odd(int val) {
	return (val / 2) + (val % 2);
}

void update_thread_progress(int& to_update) {
	SDL_LockMutex(file_mutex);
	to_update++;
	SDL_UnlockMutex(file_mutex);
}