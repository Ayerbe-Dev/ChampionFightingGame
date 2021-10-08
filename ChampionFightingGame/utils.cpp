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

	CHARA_STATUS_MAX,
};