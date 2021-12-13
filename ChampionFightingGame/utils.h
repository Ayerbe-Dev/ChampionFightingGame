#pragma once
using namespace std;
#include <functional>
#include <iostream>
#include <fstream>
#include "SDL.h"
#include <glew.h>
#include <sstream>
#include <SDL_ttf.h>
#include "GameCoordinate.fwd.h"
#include "GameCoordinate.h"

#include "Constants.h"
#include "GameTexture.h"

typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef long long i64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define TICK_RATE_S 60
#define TICK_RATE_MS 1000 / TICK_RATE_S

#define HITBOX_COUNT_MAX 9

#define PARAM_TABLE_LENGTH 256
#define ANIM_TABLE_LENGTH 256
#define CSS_SLOTS 32
#define NUM_COLS 10
#define NUM_ROWS 4
#define CSS_WIDTH WINDOW_WIDTH * 0.8
#define CSS_HEIGHT WINDOW_HEIGHT / 2.5
#define MOVE_SCRIPT_MAX 256
#define MAX_PROJECTILES 10
#define MENU_STICK_HOLD_TIMER 20
#define MENU_STICK_HOLD_INTERVAL 2
#define MAX_ANIM_LENGTH 256

#define DASH_WINDOW 14
#define TECH_WINDOW 8
#define PARRY_ADVANTAGE_FRAMES 4
#define EX_METER_SIZE 300
#define EX_METER_BARS 6

#define WINDOW_FACTOR WINDOW_HEIGHT / WINDOW_WIDTH
#define JUMP_FOLLOW_THRESHOLD 300
#define CAMERA_MAX_ZOOM_OUT 1000
#define CAMERA_MAX_ZOOM_IN 800
#define FONT_COUNT 2
#define BUFFER_WINDOW 3
#define MOTION_SPECIAL_TIMER 11
#define MAX_SOUNDS 20
#define MAX_LAYERS 5
#define FLOOR_GAMECOORD 60.0

int twoPointDistance(int x0, int y0, int x1, int y1);
void frameTimeDelay();
int clamp(int min, int value, int max);
float clampf(float min, float value, float max);
int ymlChopInt(string line);
float ymlChopFloat(string line);
string ymlChopString(string line);
bool is_collide(SDL_Rect RectA, SDL_Rect RectB);
SDL_Texture *loadTexture(const char *file_path, bool delay = true);
bool is_any_controller_input(SDL_GameController* controller);
string Filter(const string& to, const string& remove);
void draw_text(string font_name, string text, GameCoordinate pos, int font_size = 24, int r = 0, int g = 0, int b = 0, int a = 255);
void draw_text(string font_name, string text, float x_pos, float y_pos, int font_size = 24, int r = 0, int g = 0, int b = 0, int a = 255);
void draw_text_multi_lines(string font_name, string text, float x_pos, float y_pos, int font_size, int r = 0, int g = 0, int b = 0, int a = 0);
int get_blank(string s);
void refreshRenderer();
void displayLoadingScreen();
float get_relative_one_percent(float val, float denom);
int getGameSetting(string setting);
void setGameSetting(string setting, int val);
void updateGameSettings();
int round_up_odd(int val);

/*
	updateCamera(); 
	Returns a new camera because the SDL_Render func refuses to work with 
	a pointer to a camera... smh.
	
	What a looser.
*/
SDL_Rect updateCamera(int player1X, int player1Y, int player2X, int player2Y, bool no_zoom);

enum { //This is the only enum in all of our source code that I couldn't figure out how to organize lmao
	PARAM_TYPE_INT = 1,
	PARAM_TYPE_FLOAT,
	PARAM_TYPE_STRING,
	PARAM_TYPE_BOOL,

	PARAM_TYPE_MAX,
};

enum {
	SOUND_KIND_SE,
	SOUND_KIND_VC,
	SOUND_KIND_MUSIC,

	SOUND_KIND_MAX,
};

enum { //jk
	SOUND_TYPE_NORMAL,
	SOUND_TYPE_LOOP,

	SOUND_TYPE_MAX,
};

struct Offset
{
	int offset_x;
	int offset_y;
};

struct Vec4f
{
	int x;
	int y;
	int z;
	int w;
};