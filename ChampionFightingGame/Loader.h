#pragma once
#include <SDL/SDL.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "GameTextureSDL.h"

extern std::mutex file_mutex;

enum {
	PANIC_SETTING_STOP,
	PANIC_SETTING_FLY,
};

class LoadIcon {
public:
	LoadIcon();

	GameTextureSDL texture;
	bool move_right;
	bool move_down;
	bool panic_mode;
	float panic_speed;
	float panic_add;
	int panic_timer;
	int panic_setting;

	void move();
	void set_attributes();
	bool check_corner_distance();
};

class GameLoader {
public:
	GameLoader();
	GameLoader(int total_items);

	int loaded_items = 0;
	int total_items;
	int ret = 0;
	bool finished = false;
	bool can_ret = false;

	void loading_screen();
};