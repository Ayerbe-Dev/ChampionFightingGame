#pragma once
#include <SDL/SDL.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "utils.h"
#include "RenderManager.h"
#include "GameTextureSDL.h"

extern std::mutex file_mutex;


class LoadIcon {
public:
	LoadIcon();

	GameTextureSDL texture;
	bool move_right;
	bool move_down;
	bool panic_v;
	bool panic_mode;
	float panic_speed;
	float panic_add;
	bool panic_setting; //True = Gradually slows down to a stop, False = Goes flying
	int panic_timer;

	void move();
	void set_attributes();
	bool check_corner_distance(bool init);
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