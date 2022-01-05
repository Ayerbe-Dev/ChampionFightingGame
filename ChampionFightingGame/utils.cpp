using namespace std;
#include <iostream>
#include <sstream>
#include <algorithm>
#include "utils.h"
#include <SDL_image.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <chrono>
#include "GameSettings.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern SDL_Window* g_window;
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

int ymlChopInt(string line) {
	return stoi(line.substr(line.find("=") + 1));
}

float ymlChopFloat(string line) {
	return stof(line.substr(line.find("=") + 1));
}

string ymlChopString(string line) {
	return line.substr(line.find("=") + 1);
}

/// <summary>
/// SDL's rectangle collision function sucks so I made a better one.
/// </summary>
/// <param name="RectA">: The first rectangle</param>
/// <param name="RectB">: The second rectangle</param>
/// <returns>Whether or not any part of the two rectangles are touching</returns>
bool is_collide(SDL_Rect RectA, SDL_Rect RectB) {
	int ax0 = RectA.x;
	int ax1 = RectA.x + RectA.w;
	int ay0 = RectA.y;
	int ay1 = RectA.y + RectA.h;
	int bx0 = RectB.x;
	int bx1 = RectB.x + RectB.w;
	int by0 = RectB.y;
	int by1 = RectB.y + RectB.h;
	bool horizontal = (ax0 >= bx0 && ax0 <= bx1) || (ax1 >= bx0 && ax1 <= bx1) || (bx0 >= ax0 && bx0 <= ax1) || (bx1 >= ax0 && bx1 <= ax1) || (ax0 <= bx0 && ax0 >= bx1) || (ax1 <= bx0 && ax1 >= bx1) || (bx0 <= ax0 && bx0 >= ax1) || (bx1 <= ax0 && bx1 >= ax1);
	bool vertical = (ay0 <= by0 && ay0 >= by1) || (ay1 <= by0 && ay1 >= by1) || (by0 <= ay0 && by0 >= ay1) || (by1 <= ay0 && by1 >= ay1) || (ay0 >= by0 && ay0 <= by1) || (ay1 >= by0 && ay1 <= by1) || (by0 >= ay0 && by0 <= ay1) || (by1 >= ay0 && by1 <= ay1);
	return horizontal && vertical;
}

SDL_Rect updateCamera(int player1X, int player1Y, int player2X, int player2Y, bool no_zoom) {
	SDL_Rect cCamera;

	if (no_zoom) {
		cCamera.w = std::min(std::max(std::abs(player1X - player2X) + 150, 1280), 1280);
	}
	else {
		cCamera.w = std::min(std::max(std::abs(player1X - player2X) + 150, CAMERA_MAX_ZOOM_IN), CAMERA_MAX_ZOOM_OUT);
	}

	//0.5625 = WINDOW_HEIGHT / WINDOW_WIDTH. its used to scale the camera for correct proportions
	cCamera.h = cCamera.w * WINDOW_FACTOR;

	//78 is the current average size of the character.
	cCamera.x = ((player1X + player2X) / 2) - (cCamera.w / 2) + (78 / 2);

	if (cCamera.x + cCamera.w > WINDOW_WIDTH) {
		cCamera.x = WINDOW_WIDTH - cCamera.w;
	}
	else if (cCamera.x < 0) {
		cCamera.x = 0;
	}

	//559 is the absolute y value of the floor
	//JUMP_FOLLOW_THRESHOLD is the jump line before the camera starts moving
	int iYdelta = (559 - std::min(player2Y, player1Y));
	if (iYdelta >= JUMP_FOLLOW_THRESHOLD) {
		cCamera.y = WINDOW_HEIGHT - cCamera.h - iYdelta + JUMP_FOLLOW_THRESHOLD - 50;
	}
	else {
		cCamera.y = WINDOW_HEIGHT - cCamera.h - 50;
	}

	return cCamera;
}

/// <summary>
/// Loads SDL textures. This should generally be avoided during normal gameplay as everything should be loaded beforehand.
/// </summary>
/// <param name="file_path">: File path from the game's directory</param>
/// <param name="delay">: Whether or not to wait 1 frame after running, for loading thread purposes. True by default, set to false if
/// using this function to load something with no loading screen.</param>
/// <returns> The loaded texture</returns>
SDL_Texture* loadSDLTexture(const char* file_path, bool delay) {
	SDL_LockMutex(file_mutex);
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		cout << "Error loading image: " << IMG_GetError() << endl;
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(g_renderer, image_surface);
	SDL_FreeSurface(image_surface);
	SDL_UnlockMutex(file_mutex);
	if (delay) {
		frameTimeDelay();
	}
	return ret;
}

unsigned int loadGLTexture(char const* file_path) {
	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width;
	int height;
	int num_components;
	unsigned char* data = stbi_load(file_path, &width, &height, &num_components, 0);
	if (data) {
		GLenum format;
		if (num_components == 3) {
			format = GL_RGB;
		}
		else if (num_components == 4) {
			format = GL_RGBA;
		}
		else {
			format = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << file_path << std::endl;
		stbi_image_free(data);
	}

	return texture_id;
}

unsigned int loadGLTextureFromFile(const char* path, const string& directory, bool gamma) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

/// <summary>
/// Checks if a controller is making any inputs. Used for mapping a controller to a player who has none, as well as allowing any controller input to
/// skip the opening/exit certain loading screens.
/// 
/// NOTE: This function is a helper function called by PlayerInfo::is_any_inputs. That function should always be called instead of this one.
/// </summary>
/// <param name="controller">: The controller being checked.</param>
/// <returns>Whether or not the controller is making any inputs.</returns>
bool is_any_controller_input(SDL_GameController* controller) {
	for (int i = 1; i < 16; i++) {
		if (SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)i)) {
			return true;
		}
	}
	for (int i = 1; i < 7; i++) {
		if (abs(SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)i)) >= 13106) {
			return true;
		}
	}
	return false;
}

/// <summary>
/// Takes a string and removes a filtered version of it.
/// </summary>
/// <param name="to">: Base string</param>
/// <param name="remove">: Part of the string to remove</param>
/// <returns>Filtered string</returns>
string Filter(const string& to, const string& remove) {
	string ret = "";
	string ret2 = "";
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
void frameTimeDelay() {
	auto current_time = chrono::steady_clock::now();
	auto future_time = chrono::steady_clock::now() + 16.666ms;

	//reduce the future time to account for processing time
	future_time -= (current_time - g_chron);

	while (current_time < future_time) {
		current_time = chrono::steady_clock::now();
	}
	g_chron = chrono::steady_clock::now();
};

//Draw text, center-oriented, based on 0,0 = Bottom middle coords
void draw_text(string font_name, string text, GameCoordinate pos, int font_size, int r, int g, int b, int a) {
	TTF_Font* font = TTF_OpenFont(font_name.c_str(), font_size);
	if (!font) {
		printf("Failed to load font:  %s\n", TTF_GetError());
	}
	SDL_Color color = { r,g,b,a };
	SDL_Surface* text_surface = TTF_RenderText_Blended(font, text.c_str(), color);

	if (!text_surface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);
	SDL_FreeSurface(text_surface);

	int text_width, text_height;
	SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

	SDL_Rect render_rect;
	render_rect.x = pos.x + (WINDOW_WIDTH / 2);
	render_rect.y = (pos.y - WINDOW_HEIGHT) * -1;
	render_rect.w = text_width;
	render_rect.h = text_height;
	render_rect.x -= render_rect.w / 2;
	render_rect.y -= render_rect.h / 2;

	SDL_RenderCopy(g_renderer, texture, nullptr, &render_rect);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

//Draw text, center-oriented, based on 0,0 = Top left corner coords
void draw_text(string font_name, string text, float x_pos, float y_pos, int font_size, int r, int g, int b, int a) {
	TTF_Font* font = TTF_OpenFont(font_name.c_str(), font_size);
	if (!font) {
		printf("Failed to load font:  %s\n", TTF_GetError());
	}
	SDL_Color color = { r,g,b,a };
	SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), color);

	if (!text_surface) {
		printf("Failed to render text:  %s\n", TTF_GetError());
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, text_surface);
	SDL_FreeSurface(text_surface);

	int text_width, text_height;
	SDL_QueryTexture(texture, nullptr, nullptr, &text_width, &text_height);

	SDL_Rect render_rect;
	render_rect.x = x_pos;
	render_rect.y = y_pos;
	render_rect.w = text_width;
	render_rect.h = text_height;
	render_rect.x -= render_rect.w / 2;
	render_rect.y -= render_rect.h / 2;

	SDL_RenderCopy(g_renderer, texture, nullptr, &render_rect);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

//Take a string and divide each word from it into multiple lines (Planned to be used for the CSS)
void draw_text_multi_lines(string font_name, string text, float x_pos, float y_pos, int font_size, int r, int g, int b, int a) {
	int blank_pos = get_blank(text);
	if (blank_pos) {
		string first_text = text.substr(0, blank_pos);
		string second_text = text.substr(blank_pos + 1);
		draw_text(font_name, first_text, x_pos, y_pos, font_size, r, g, b, a);
		draw_text_multi_lines(font_name, second_text, x_pos, y_pos + font_size, font_size, r, g, b, a);
	}
	else {
		draw_text(font_name, text, x_pos, y_pos, font_size, r, g, b, a);
	}
}

//Check the first character in a string that is a space
int get_blank(string s) {
	const char* c = s.c_str();
	for (int i = 0; i < s.length(); i++) {
		if (isblank(c[i])) {
			return i;
		}
	}
	return 0;
}

/// <summary>
/// T H E   C Y C L E   O F   D E S T R U C T I O N   A N D   R E C R E A T I O N
/// </summary>
void refreshRenderer() {
	SDL_RenderClear(g_renderer);
	SDL_DestroyRenderer(g_renderer);
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
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

void displayLoadingScreen() {
	SDL_RenderClear(g_renderer);
	GameTexture loadingSplash;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	loadingSplash.render();
	SDL_RenderPresent(g_renderer);
	loadingSplash.clearTexture();
}

float get_relative_one_percent(float val, float denom) {
	float mul = denom / 100.0;
	return (val / 100.0) * mul;
}

//These aren't member funcs because this way even though we technically use a global var, it's entirely used in this file

GameSettings g_settings;

/// <summary>
/// Check the value of a specific setting. If the user passes an invalid setting, return 0. TODO: Make this a GameManager member function.
/// </summary>
/// <param name="setting">: The setting to check.</param>
/// <returns>Value of a setting</returns>
int getGameSetting(string setting) {
	for (int i = 0; i < SETTING_KIND_MAX; i++) {
		if (g_settings.settings[i].name == setting) {
			return g_settings.settings[i].val;
		}
	}
	return 0;
}

/// <summary>
/// Change the setting value on the settings table to the specified value. Does NOT update the settings param file.
/// 
/// Use this function when the user has set a value (such as screen resolution), but has not confirmed their selection yet.
/// </summary>
/// <param name="setting">: The setting to change.</param>
/// <param name="val">: What to change the new value to.</param>
void setGameSetting(string setting, int val) {
	for (int i = 0; i < SETTING_KIND_MAX; i++) {
		if (g_settings.settings[i].name == setting) {
			g_settings.settings[i].val = val;
			return;
		}
	}
}

/// <summary>
/// Update the settings file to match our current list of settings. Use this whenever settings are confirmed.
/// </summary>
void updateGameSettings() {
// .                    ... ............ ....... ...............................
//.... ... . .   .   . ... .....................................................
//.... . . .       ..................,,***********,.............................
// ... ..  . ...................,********************/*,........................
//. .      .    .............,*////*****,*,,,,,,,******//*,.....................
// ...  .. ............... **////*/*****,*,,,,,********////*....................
// . . . .................*/////////*****,**************////*,..................
//.......................*//((////*****,,,,,,,,,,,******/////*..................
//   .  .... ...........,//(((///****,,,,,,,,,,,,*******//////*.................
// ...  . ..............*/((((////////////*/*//((###((((/*////**................
// . . ..... ...........*//(((((##%%%%%%%#((((%%%&&%#%&&%%#/////................
//.... .................*//((#%%%&&%&&&&&%#**(%&&&(%@%(%%%##///(.*/,............
// . . ... . ............*/(#(%&%(////((((*,,//(((((##((**#(///(/#%,............
// ....... ..........((((//(#((//////////(*,*///*********/((//((##(,............
// . . ...............###//((//*******////*,,*/**/(/******/((/(//(/.............
// ....... ...........((#(/((//////(###((#((((##((//#%#((//(((((//(.............
//.     ..............(/(((((((((#%#((#%&&&%%%&&&#((((####(((((((/*.............
//   . ................(((#(((((#%#%%%%%%%&&%#&#/(###&&&(((#(#..................
// ......................../(##(/(##&@&&%%%#%%/((##&%//*/(%(((..................
//..........................(###(////((((((/,,*/////**//##(##...................
//..........................*(####/(//((((((((////(/*//##(##/...................
// .........................*(((###((/(((((((((//(////(#(#(#....................
//.........................,,((#(#(#((//*//*********/(#((##(....................
//.......................,.,/(###(####(((//******///(######(....................
//.......................,,,((############(((((#####%#%##((/....................
//..................,,.,,,,,((((########%%%%%%%%%%%%%###(((*....................
//...................,,,,,,,((((((((####%####%%%%%####(((((/....................
//.................,,,,,,,*/(((((((((((##############(((((((*,..................
//..................,,,,/&&(///(((((((((###########((((((//(/%#*................
//.................,.,.,((&((////((((((((((((######((((((///(%*/(...............
//..................,,..(/(%(////((((((((((((###(((((((((((/((**(/..............
//................,,,,..*/////&/*/(((((((((((((((((((////(%#/****#*....,.. .....
//.................,,,,..*/***,*//%#(((//((((////////*#%///******//...,,.   ....
//....................,,..**********/(#&%(/(((///%%(**************,..,,..   ....
// ........,*,.... ....,...,,*****//*****/. ,/********************...,....   ...
//........,///***,.... ......*****,,*****(*(******,***********,,,...,....    ...
//.........,,,/(/**,...........,**,************,,*,*********,,,.........   .../,
//.............,,*(#######%........,,,***********,,,,,,,**,,,. .........   .#(*,
//................,,,**,,,,((,........,***********,,,,,,,,,......,....   (##(/*,

			/*Mark is watching. Mark does not approve of globals.*/


	ofstream settings;
	settings.open("resource/save/game_settings.yml", std::ofstream::trunc);
	for (int i = 0; i < SETTING_KIND_MAX; i++) {
		settings << g_settings.settings[i].name << " " << g_settings.settings[i].val << endl;
	}
	settings.close();
}

int round_up_odd(int val) {
	return (val / 2) + (val % 2);
}

mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from) {
	mat4 to;
	to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}