#pragma once
using namespace std;

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
#define WINDOW_W_FACTOR WINDOW_WIDTH / WINDOW_HEIGHT
#define JUMP_FOLLOW_THRESHOLD 300
#define CAMERA_MAX_ZOOM_OUT 1920
#define CAMERA_MAX_ZOOM_IN 800
#define FONT_COUNT 2
#define BUFFER_WINDOW 3
#define MOTION_SPECIAL_TIMER 11
#define MAX_SOUNDS 20
#define MAX_LAYERS 5
#define FLOOR_GAMECOORD 1.0
#define MAX_SHADERS 4

int twoPointDistance(int x0, int y0, int x1, int y1);
void frameTimeDelay();
int clamp(int min, int value, int max);
float clampf(float min, float value, float max);
int ymlChopInt(string line);
float ymlChopFloat(string line);
string ymlChopString(string line);
SDL_Texture *loadSDLTexture(const char *file_path, bool delay = true);
unsigned int loadGLTexture(const char* file_path);
unsigned int loadGLTextureFromFile(const char* file_path, const string& directory, bool gamma = false);
bool is_any_controller_input(SDL_GameController* controller);
string Filter(const string& to, const string& remove);
void draw_text(string font_name, string text, GameCoordinate pos, int font_size = 24, int r = 0, int g = 0, int b = 0, int a = 255);
void draw_text(string font_name, string text, float x_pos, float y_pos, int font_size = 24, int r = 0, int g = 0, int b = 0, int a = 255);
void draw_text_multi_lines(string font_name, string text, float x_pos, float y_pos, int font_size, int r = 0, int g = 0, int b = 0, int a = 0);
int get_blank(string s);
void refreshRenderer();
float get_relative_one_percent(float val, float denom);
int getGameSetting(string setting);
void setGameSetting(string setting, int val);
void updateGameSettings();
int round_up_odd(int val);
void update_thread_progress(int& to_update);

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