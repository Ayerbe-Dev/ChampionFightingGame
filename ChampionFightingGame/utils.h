#pragma once
#include <iostream>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_DEPTH 2000
#define WINDOW_FACTOR WINDOW_HEIGHT / WINDOW_WIDTH
#define WINDOW_W_FACTOR WINDOW_WIDTH / WINDOW_HEIGHT
#define SDL_WINDOW_WIDTH (WINDOW_WIDTH / 2)
#define SDL_WINDOW_HEIGHT (WINDOW_HEIGHT / 2)

#define FONT_COUNT 2
#define MAX_SOUNDS 20
#define MAX_SHADERS 4
#define FORCE_INLINE [[gnu::always_inline]] inline

int twoPointDistance(int x0, int y0, int x1, int y1);
void wait_ms(double ms_duration = 16.667, bool process_time = true);
int clamp(int min, int value, int max);
float clampf(float min, float value, float max);
int ymlChopInt(std::string line);
float ymlChopFloat(std::string line);
std::string ymlChopString(std::string line);
std::string Filter(const std::string& to, const std::string& remove);
int get_blank(std::string s);
float get_relative_one_percent(float val, float denom);
int round_up_odd(int val);
void update_thread_progress(int& to_update);
void print_init();
int rng(const int& min, const int& max);