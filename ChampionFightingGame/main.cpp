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
int registered_controllers[2] = { -1, -1 };
bool debug = false;
bool can_play_non_music = true;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
SoundManager g_soundmanager;
auto g_chron = chrono::steady_clock::now();
SDL_mutex* mutex;

int main() {	

	//Initialize SDL

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}
	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
	}

	//Create the window and the renderer

	g_window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE /* | SDL_WINDOW_FULLSCREEN_DESKTOP*/);
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	mutex = SDL_CreateMutex();

	//Display the loading screen before anything else can happen

	displayLoadingScreen();

	bool running = true;
	int game_state = GAME_STATE_DEBUG_MENU;

	//Initialize controller input

	SDL_GameControllerEventState(SDL_ENABLE);

	//Initialize audio

	SDL_AudioSpec format;
	format.freq = 22050;
	format.format = AUDIO_F32SYS;
	format.channels = 2;
	format.samples = 361; 
	/*
	The number of samples needs to be a perfect square, and that value represents how much data will go into each iteration of the callback. Since the
	callback only runs when the program determines it is time to load in more audio data, this value also effectively sets how often the callback will
	run. The value of 361 (19*19) causes the callback to run ~61 times per second, which allows it to sync up with frame advance.
	*/
	format.callback = audio_callback;
	format.userdata = NULL;
	if (SDL_OpenAudio(&format, NULL) < 0) {
		printf("Error opening SDL_audio: %s\n", SDL_GetError());
	}
	SDL_PauseAudio(0);

	g_soundmanager = SoundManager(true);

	//Initialize player info

	PlayerInfo player_info[2];
	player_info[0] = PlayerInfo(0);
	player_info[1] = PlayerInfo(1);

	while (running) {
		refreshRenderer();

		if (game_state == GAME_STATE_GAME) {
			game_state = game_main(player_info);
			if (game_state == GAME_STATE_CLOSE) {
				running = false;
			}
		}
		else if (game_state == GAME_STATE_MENU) {
			game_state = menu_main(player_info);
			if (game_state == GAME_STATE_CLOSE) {
				running = false;
			}
		}
		else if (game_state == GAME_STATE_CHARA_SELECT) {
			game_state = chara_select_main(player_info);
			if (game_state == GAME_STATE_CLOSE) {
				running = false;
			}
		}
		else if (game_state == GAME_STATE_CLOSE) {
			running = false;
		}
		else {
			game_state = debugMenu(player_info, game_state);
			if (game_state == GAME_STATE_CLOSE) {
				running = false;
			}
		}
	}

	SDL_DestroyWindow(g_window);
	SDL_DestroyMutex(mutex);

	SDL_Quit();

	return 0;
}