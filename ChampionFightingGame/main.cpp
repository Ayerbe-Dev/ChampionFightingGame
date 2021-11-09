#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "Game.h"
#include "Menu.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "DebugMenu.h"
#include "CharaSelect.h"
#include "SoundManager.h"
#undef main
using namespace std;
int registered_controllers[2] = {-1, -1};
bool debug = false;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
SoundManager* g_soundmanager;
SDL_AudioSpec format;

int main() {
	bool running = true;
	int game_state = GAME_STATE_DEBUG_MENU;

	format.freq = 22050;
	format.format = AUDIO_S16;
	format.channels = 2;
	format.samples = 4096;
	format.callback = audio_callback;
	format.userdata = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}
	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
	}
	SDL_AudioDeviceID dev_id = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, 0), 0, &format, NULL, 0);
	if (dev_id < 0) {
		printf("Error opening SDL_audio: %s\n", SDL_GetError());
	}
	SDL_PauseAudioDevice(dev_id, 0);
	SDL_GameControllerEventState(SDL_ENABLE);
	g_window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE /* | SDL_WINDOW_FULLSCREEN_DESKTOP*/);
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	g_soundmanager = new SoundManager(true);
	PlayerInfo player_info[2];
	player_info[0] = PlayerInfo(0);
	player_info[1] = PlayerInfo(1);

	while (running) {
		refreshRenderer();
		SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		if (game_state == GAME_STATE_GAME) {
			game_state = game_main(player_info);
		}
		else if (game_state == GAME_STATE_MENU) {
			game_state = menu_main(player_info);
		}
		else if (game_state == GAME_STATE_CHARA_SELECT) {
			game_state = chara_select_main(player_info);
		}
		else if (game_state == GAME_STATE_CLOSE) {
			running = false;
		}
		else {
			game_state = debugMenu(player_info, game_state);
		}
	}

	delete g_soundmanager;
	SDL_DestroyWindow(g_window);

	SDL_Quit();

	return 0;
}