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
	CHARA_SITUATION_GROUND,
	CHARA_SITUATION_AIR,
	CHARA_SITUATION_DOWN,

	CHARA_SITUATION_MAX,
};