#pragma warning(disable : 4996)
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <Windows.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <glew/glew.h>
#include "SDL Helpers.h"
#include "utils.h"
#include "GameSettings.h"
#include "GameStates.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "BattleObjectManager.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "RenderManager.h"
#include "stb_image.h"
//Windows.h has a constant named LoadIcon, while Loader.h has a class named LoadIcon. C++ will always assume we mean the constant, so we need to 
//undefine it before we include Loader.h.
#undef LoadIcon
#include "Loader.h"
#undef main

int registered_controllers[2] = { -1, -1 };
bool debug = false;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_GLContext g_context;

std::mutex file_mutex;

void initialize_SDL();
void initialize_GLEW();

int main() {
	//Hide the console window. It'll be a LONG time before we get to uncomment these lines

//	HWND windowHandle = GetConsoleWindow();
//	ShowWindow(windowHandle, SW_HIDE);

	initialize_SDL();
	initialize_GLEW();

	RenderManager* render_manager = RenderManager::get_instance();
	SoundManager* sound_manager = SoundManager::get_instance();
	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();	
	GameManager game_manager;

	render_manager->init();

	bool running = opening_main(&game_manager);

	while (running) {
		refreshRenderer();
		for (int i = 0; i < MAX_LAYERS; i++) {
			game_manager.looping[i] = true;
		}
		if (game_manager.game_main[*game_manager.game_state] != nullptr) {
			game_manager.game_main[*game_manager.game_state](&game_manager);
		}
		else if (*game_manager.game_state != GAME_STATE_CLOSE) {
			char buffer[86];
			sprintf(buffer, "Error: Game State was %d (not GAME_STATE_CLOSE) but there was no associated function!", *game_manager.game_state);
			game_manager.player_info[0]->crash_reason = buffer;
			game_manager.game_main[GAME_STATE_DEBUG_MENU](&game_manager);
		}

		if (*game_manager.game_state == GAME_STATE_CLOSE) {
			running = false;
		}
	}

	game_manager.~GameManager();
	sound_manager->unloadSoundAll();
	SDL_DestroyWindow(g_window);
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyRenderer(g_renderer);

	SDL_Quit();

//	ShowWindow(windowHandle, SW_SHOW);
//	If we try to end the program without putting the window back up, it technically causes a crash. Program was about to end anyway so it doesn't really
//	matter but ehhhhhhhhh, clean exits are nice
	exit(0);
//	Same goes for this line; causes a crash on exit if we return 0 normally which wouldn't normally be a problem but again, clean exits
//	Don't ask me why exit(0) is ok but return 0 isn't, I cannot answer that question.
}

void initialize_SDL() {
	//Initialize Most of SDL

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
	}

	//Initialize TTF

	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
	}

	//Initialize Controllers

	SDL_GameControllerEventState(SDL_ENABLE);


	//Create the window

	if (getGameSetting("fullscreen")) {
		g_window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getGameSetting("res_x"), getGameSetting("res_y"), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		g_window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, getGameSetting("res_x"), getGameSetting("res_y"), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}

	//Initialize a bunch of globals

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	g_context = SDL_GL_CreateContext(g_window);

	//Initialize Audio

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
}

void initialize_GLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW!" << std::endl;
	}
	SDL_GL_MakeCurrent(g_window, g_context);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);
}