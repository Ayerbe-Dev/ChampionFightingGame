#pragma once
#include <iostream>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define TICK_RATE_S 60
#define TICK_RATE_MS 1000 / TICK_RATE_S

#define PARAM_TABLE_LENGTH 256
#define ANIM_TABLE_LENGTH 256
#define CSS_SLOTS 32
#define NUM_COLS 10
#define NUM_ROWS 4
#define CSS_WIDTH WINDOW_WIDTH * 0.8
#define CSS_HEIGHT WINDOW_HEIGHT / 2.5
#define MOVE_SCRIPT_MAX 256
#define MAX_PROJECTILES 10
#define MAX_ANIM_LENGTH 256

#define DASH_WINDOW 14
#define TECH_WINDOW 8
#define PARRY_ADVANTAGE_FRAMES 4
#define EX_METER_SIZE 300
#define EX_METER_BARS 6

#define WINDOW_FACTOR WINDOW_HEIGHT / WINDOW_WIDTH
#define WINDOW_W_FACTOR WINDOW_WIDTH / WINDOW_HEIGHT
#define JUMP_FOLLOW_THRESHOLD 300
#define CAMERA_MAX_ZOOM_OUT 1920
#define CAMERA_MAX_ZOOM_IN 800
#define FONT_COUNT 2
#define BUFFER_WINDOW 3
#define MOTION_SPECIAL_TIMER 11
#define MAX_SOUNDS 20
#define FLOOR_GAMECOORD 1.0
#define MAX_SHADERS 4

int twoPointDistance(int x0, int y0, int x1, int y1);
void wait_ms(double ms_duration = 16.667);
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