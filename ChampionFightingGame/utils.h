using namespace std;
#include <iostream>
#include <sstream>
#pragma once
typedef char i8;
typedef unsigned char u8;
typedef short i16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int i32;
typedef long long i64;
typedef unsigned long long u64;
typedef float f32;

#define global_variable static
#define internal static

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TICK_RATE_S 60
#define TICK_RATE_MS 1000 / TICK_RATE_S

int clamp(int min, int value, int max);


enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_LP,
	BUTTON_MP,
	BUTTON_HP,
	BUTTON_LK,
	BUTTON_MK,
	BUTTON_HK,
	BUTTON_START,

	BUTTON_MAX,
};

enum {
	CHARA_STATUS_WAIT,
	CHARA_STATUS_WALKF,
	CHARA_STATUS_WALKB,
	CHARA_STATUS_DASH,
	CHARA_STATUS_DASHB,
	CHARA_STATUS_CROUCHD,
	CHARA_STATUS_CROUCH,
	CHARA_STATUS_CROUCHU,
	CHARA_STATUS_JUMPSQUAT,
	CHARA_STATUS_JUMP,
	CHARA_STATUS_ATTACK,
	CHARA_STATUS_HITSTUN,
	CHARA_STATUS_BLOCKSTUN,

	CHARA_STATUS_MAX,
};

enum {
	DASH_CANCEL_KIND_FOLLOWUP,
	DASH_CANCEL_KIND_INDEFINITE,
	DASH_CANCEL_KIND_MAX,
};

enum {
	CHARA_SITUATION_GROUND,
	CHARA_SITUATION_AIR,
	CHARA_SITUATION_DOWN,

	CHARA_SITUATION_MAX,
};