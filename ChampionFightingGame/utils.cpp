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

SoundInfo sounds[3][MAX_SOUNDS];
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern bool debug;
extern bool can_play_non_music;

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

SDL_Texture* loadTexture(const char* file_path) {
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		cout << "Error loading image: " << IMG_GetError() << endl;
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(g_renderer, image_surface);
	SDL_FreeSurface(image_surface);
	return ret;
}

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


string Filter(const string& to, const string& remove) {
	string ret = "";
	string ret2 = "";
	int removal_index = to.find(remove);
	int cont_index = removal_index + remove.length();
	ret = to.substr(0, removal_index);
	ret2 = to.substr(cont_index, to.length());

	return ret + ret2;
}

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

void frameTimeDelay() {
	auto current_time = chrono::steady_clock::now();
	auto future_time = chrono::steady_clock::now() + 16.667ms;
	while (current_time < future_time) {
		current_time = chrono::steady_clock::now();
	}
};
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

void audio_callback(void* unused, Uint8* stream, int len) {

	int i;
	Uint32 amount;
	SDL_memset(stream, 0, len);

	for (i = 0; i < MAX_SOUNDS; i++) {
		for (int i2 = 0; i2 < 3; i2++) {
			if (sounds[i2][i].data) {
				if (can_play_non_music  || (i == 0 && i2 == 2) ) { //The music should always be put into the first spot of sound id 2, so this statement 
						//basically serves to check "are we looking at the music"
					cout << "Position in song: " << sounds[i2][i].dpos << endl;
					amount = (sounds[i2][i].dlen - sounds[i2][i].dpos);
					if (amount > len) {
						amount = len;
					}
					SDL_MixAudio(stream, &sounds[i2][i].data[sounds[i2][i].dpos], amount, SDL_MIX_MAXVOLUME);
					sounds[i2][i].dpos += amount;
				}
			}
		}
	}
}

void addSoundToIndex(char* file, int* ret, int id) {
	int index;
	SDL_AudioSpec wave;
	Uint8* data;
	Uint32 dlen;
	SDL_AudioCVT cvt;

	for (index = 0; index < MAX_SOUNDS; index++) {
		if (!sounds[id][index].data) {
			break;
		}
	}
	*ret = index;
	if (index == MAX_SOUNDS) {
		return;
	}

	if (SDL_LoadWAV(file, &wave, &data, &dlen) == NULL) { //Load the WAV
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return;
	}
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_S16, 2, 22050);

	cvt.len = dlen;
	cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult); //Converting the audio goes through multiple passes, some of which increase the size, so
	//we allocate enough memory to handle the size during the largest pass
	SDL_memcpy(cvt.buf, data, dlen);

	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);

	SDL_LockAudio();
	sounds[id][index].data = cvt.buf;
	sounds[id][index].dlen = cvt.len_cvt;
	sounds[id][index].dpos = 0;
	SDL_UnlockAudio();
}

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
	loadingSplash.init("resource/ui/menu/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	loadingSplash.render();
	SDL_RenderPresent(g_renderer);
	loadingSplash.clearTexture();
}