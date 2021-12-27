#pragma warning(disable : 4996)
#include <chrono>
#include <thread>
#include <iostream>
#include <Windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "GameSettings.h"
#include "GameStates.h"
#include "Animation.h"
#include "Debugger.h"
#include "Stage.h"
#include "UI.h"
#include "SoundManager.h"
#include "GameManager.h"
//Windows.h has a constant named LoadIcon, while Loader.h has a class named LoadIcon. C++ will always assume we mean the constant, so we need to 
//undefine it before we include Loader.h.
#undef LoadIcon
#include "Loader.h"
#undef main

using namespace std;

int registered_controllers[2] = { -1, -1 };
bool debug = false;
SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_GLContext g_context;
SoundManager g_soundmanager;
auto g_chron = chrono::steady_clock::now();
SDL_mutex* mutex;

void initialize_SDL();
void initialize_GLEW();

int main() {
	//Hide the console window. It'll be a LONG time before we get to uncomment these lines

//	HWND windowHandle = GetConsoleWindow();
//	ShowWindow(windowHandle, SW_HIDE);

	initialize_SDL();
	initialize_GLEW();

	GameManager game_manager;

	bool running = opening_main(&game_manager);

	while (running) {
		refreshRenderer();
		for (int i = 0; i < MAX_LAYERS; i++) {
			*game_manager.looping[i] = true;
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
	g_soundmanager.unloadSoundAll();
	SDL_DestroyWindow(g_window);
	SDL_GL_DeleteContext(g_context);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyMutex(mutex);

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
	mutex = SDL_CreateMutex();

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

	g_soundmanager = SoundManager(true);
}

void initialize_GLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW!" << endl;
	}
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void example_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	Debugger debugger;
	debugger = Debugger();

	
	GameLoader *game_loader = new GameLoader;
	game_loader->player_info[0] = player_info[0];
	game_loader->player_info[1] = player_info[1];


	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool loading = true;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

/*

 /$$$$$$$                      /$$                                     /$$                                 /$$ /$$
| $$__  $$                    | $$                                    | $$                                | $$|__/
| $$  \ $$  /$$$$$$   /$$$$$$$| $$  /$$$$$$   /$$$$$$   /$$$$$$       | $$        /$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$$   /$$$$$$
| $$  | $$ /$$__  $$ /$$_____/| $$ |____  $$ /$$__  $$ /$$__  $$      | $$       /$$__  $$ |____  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
| $$  | $$| $$$$$$$$| $$      | $$  /$$$$$$$| $$  \__/| $$$$$$$$      | $$      | $$  \ $$  /$$$$$$$| $$  | $$| $$| $$  \ $$| $$  \ $$
| $$  | $$| $$_____/| $$      | $$ /$$__  $$| $$      | $$_____/      | $$      | $$  | $$ /$$__  $$| $$  | $$| $$| $$  | $$| $$  | $$
| $$$$$$$/|  $$$$$$$|  $$$$$$$| $$|  $$$$$$$| $$      |  $$$$$$$      | $$$$$$$$|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
|_______/  \_______/ \_______/|__/ \_______/|__/       \_______/      |________/ \______/  \_______/ \_______/|__/|__/  |__/ \____  $$
																															 /$$  \ $$
																															|  $$$$$$/
																															 \______/
 /$$$$$$$$ /$$                                           /$$       /$$    /$$                    /$$           /$$       /$$
|__  $$__/| $$                                          | $$      | $$   | $$                   |__/          | $$      | $$
   | $$   | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$$      | $$   | $$ /$$$$$$   /$$$$$$  /$$  /$$$$$$ | $$$$$$$ | $$  /$$$$$$   /$$$$$$$
   | $$   | $$__  $$ /$$__  $$ /$$__  $$ |____  $$ /$$__  $$      |  $$ / $$/|____  $$ /$$__  $$| $$ |____  $$| $$__  $$| $$ /$$__  $$ /$$_____/
   | $$   | $$  \ $$| $$  \__/| $$$$$$$$  /$$$$$$$| $$  | $$       \  $$ $$/  /$$$$$$$| $$  \__/| $$  /$$$$$$$| $$  \ $$| $$| $$$$$$$$|  $$$$$$
   | $$   | $$  | $$| $$      | $$_____/ /$$__  $$| $$  | $$        \  $$$/  /$$__  $$| $$      | $$ /$$__  $$| $$  | $$| $$| $$_____/ \____  $$
   | $$   | $$  | $$| $$      |  $$$$$$$|  $$$$$$$|  $$$$$$$         \  $/  |  $$$$$$$| $$      | $$|  $$$$$$$| $$$$$$$/| $$|  $$$$$$$ /$$$$$$$/
   |__/   |__/  |__/|__/       \_______/ \_______/ \_______/          \_/    \_______/|__/      |__/ \_______/|_______/ |__/ \_______/|_______/



 /$$   /$$
| $$  | $$
| $$  | $$  /$$$$$$   /$$$$$$   /$$$$$$
| $$$$$$$$ /$$__  $$ /$$__  $$ /$$__  $$
| $$__  $$| $$$$$$$$| $$  \__/| $$$$$$$$
| $$  | $$| $$_____/| $$      | $$_____/
| $$  | $$|  $$$$$$$| $$      |  $$$$$$$
|__/  |__/ \_______/|__/       \_______/

*/

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadGame, "Init.rar", (void*)game_loader);
	SDL_DetachThread(loading_thread);
	
	game_manager->set_menu_info(nullptr);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (loading) {
		frameTimeDelay();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return game_manager->update_state(GAME_STATE_CLOSE);
				}
				break;
			}
		}

		load_icon.move();
		SDL_LockMutex(mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		int total_items = 17; //Change to reflect the actual number of items
		loadingBar.setTargetPercent(((float)game_loader->loaded_items / total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(mutex);

		if (game_loader->finished) {
			if (!game_loader->can_ret) {
/*

 /$$$$$$$$             /$$                                    /$$           /$$                                 /$$ /$$
| $$_____/            | $$                                   | $$          | $$                                | $$|__/
| $$       /$$   /$$ /$$$$$$    /$$$$$$  /$$$$$$   /$$$$$$$ /$$$$$$        | $$        /$$$$$$   /$$$$$$   /$$$$$$$ /$$ /$$$$$$$   /$$$$$$
| $$$$$   |  $$ /$$/|_  $$_/   /$$__  $$|____  $$ /$$_____/|_  $$_/        | $$       /$$__  $$ |____  $$ /$$__  $$| $$| $$__  $$ /$$__  $$
| $$__/    \  $$$$/   | $$    | $$  \__/ /$$$$$$$| $$        | $$          | $$      | $$  \ $$  /$$$$$$$| $$  | $$| $$| $$  \ $$| $$  \ $$
| $$        >$$  $$   | $$ /$$| $$      /$$__  $$| $$        | $$ /$$      | $$      | $$  | $$ /$$__  $$| $$  | $$| $$| $$  | $$| $$  | $$
| $$$$$$$$ /$$/\  $$  |  $$$$/| $$     |  $$$$$$$|  $$$$$$$  |  $$$$/      | $$$$$$$$|  $$$$$$/|  $$$$$$$|  $$$$$$$| $$| $$  | $$|  $$$$$$$
|________/|__/  \__/   \___/  |__/      \_______/ \_______/   \___/        |________/ \______/  \_______/ \_______/|__/|__/  |__/ \____  $$
																																  /$$  \ $$
																																 |  $$$$$$/
																																  \______/
 /$$$$$$$$ /$$                                           /$$       /$$    /$$                    /$$           /$$       /$$
|__  $$__/| $$                                          | $$      | $$   | $$                   |__/          | $$      | $$
   | $$   | $$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$$      | $$   | $$ /$$$$$$   /$$$$$$  /$$  /$$$$$$ | $$$$$$$ | $$  /$$$$$$   /$$$$$$$
   | $$   | $$__  $$ /$$__  $$ /$$__  $$ |____  $$ /$$__  $$      |  $$ / $$/|____  $$ /$$__  $$| $$ |____  $$| $$__  $$| $$ /$$__  $$ /$$_____/
   | $$   | $$  \ $$| $$  \__/| $$$$$$$$  /$$$$$$$| $$  | $$       \  $$ $$/  /$$$$$$$| $$  \__/| $$  /$$$$$$$| $$  \ $$| $$| $$$$$$$$|  $$$$$$
   | $$   | $$  | $$| $$      | $$_____/ /$$__  $$| $$  | $$        \  $$$/  /$$__  $$| $$      | $$ /$$__  $$| $$  | $$| $$| $$_____/ \____  $$
   | $$   | $$  | $$| $$      |  $$$$$$$|  $$$$$$$|  $$$$$$$         \  $/  |  $$$$$$$| $$      | $$|  $$$$$$$| $$$$$$$/| $$|  $$$$$$$ /$$$$$$$/
   |__/   |__/  |__/|__/       \_______/ \_______/ \_______/          \_/    \_______/|__/      |__/ \_______/|_______/ |__/ \_______/|_______/

 /$$   /$$
| $$  | $$
| $$  | $$  /$$$$$$   /$$$$$$   /$$$$$$
| $$$$$$$$ /$$__  $$ /$$__  $$ /$$__  $$
| $$__  $$| $$$$$$$$| $$  \__/| $$$$$$$$
| $$  | $$| $$_____/| $$      | $$_____/
| $$  | $$|  $$$$$$$| $$      |  $$$$$$$
|__/  |__/ \_______/|__/       \_______/

*/

//				game_manager->set_menu_info(&menu);

//				These lines are applicable to all game states where the GameManager is in charge of menus, just replace "menu" with the actual class
			}
			game_loader->can_ret = true;

			//For most game_main functions, this entire section can be replaced with just "loading = false;"

			SDL_PumpEvents();
			keyboard_state = SDL_GetKeyboardState(NULL);

			for (int i = 0; i < 2; i++) {
				player_info[i]->poll_buttons(keyboard_state);
				if (player_info[i]->is_any_inputs()) {
					loading = false;
				}
			}
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (*game_manager->looping) {
		frameTimeDelay();
		SDL_RenderClear(g_renderer);
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}

/*
		 /$$   /$$           /$$                                      /$$$$$$                  /$$
		| $$  | $$          |__/                                     /$$__  $$                | $$
		| $$  | $$ /$$$$$$$  /$$  /$$$$$$  /$$   /$$  /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$$  /$$$$$$
		| $$  | $$| $$__  $$| $$ /$$__  $$| $$  | $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$ /$$__  $$
		| $$  | $$| $$  \ $$| $$| $$  \ $$| $$  | $$| $$$$$$$$      | $$      | $$  \ $$| $$  | $$| $$$$$$$$
		| $$  | $$| $$  | $$| $$| $$  | $$| $$  | $$| $$_____/      | $$    $$| $$  | $$| $$  | $$| $$_____/
		|  $$$$$$/| $$  | $$| $$|  $$$$$$$|  $$$$$$/|  $$$$$$$      |  $$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$
		 \______/ |__/  |__/|__/ \____  $$ \______/  \_______/       \______/  \______/  \_______/ \_______/
									  | $$
									  | $$
									  |__/
		  /$$$$$$                                      /$$   /$$
		 /$$__  $$                                    | $$  | $$
		| $$  \__/  /$$$$$$   /$$$$$$   /$$$$$$$      | $$  | $$  /$$$$$$   /$$$$$$   /$$$$$$
		| $$ /$$$$ /$$__  $$ /$$__  $$ /$$_____/      | $$$$$$$$ /$$__  $$ /$$__  $$ /$$__  $$
		| $$|_  $$| $$  \ $$| $$$$$$$$|  $$$$$$       | $$__  $$| $$$$$$$$| $$  \__/| $$$$$$$$
		| $$  \ $$| $$  | $$| $$_____/ \____  $$      | $$  | $$| $$_____/| $$      | $$_____/
		|  $$$$$$/|  $$$$$$/|  $$$$$$$ /$$$$$$$/      | $$  | $$|  $$$$$$$| $$      |  $$$$$$$
		 \______/  \______/  \_______/|_______/       |__/  |__/ \_______/|__/       \_______/
*/

		SDL_SetRenderTarget(g_renderer, pScreenTexture);

/*
		 /$$$$$$$                            /$$                            /$$$$$$                  /$$
		| $$__  $$                          | $$                           /$$__  $$                | $$
		| $$  \ $$  /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$   /$$$$$$       | $$  \__/  /$$$$$$   /$$$$$$$  /$$$$$$
		| $$$$$$$/ /$$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$      | $$       /$$__  $$ /$$__  $$ /$$__  $$
		| $$__  $$| $$$$$$$$| $$  \ $$| $$  | $$| $$$$$$$$| $$  \__/      | $$      | $$  \ $$| $$  | $$| $$$$$$$$
		| $$  \ $$| $$_____/| $$  | $$| $$  | $$| $$_____/| $$            | $$    $$| $$  | $$| $$  | $$| $$_____/
		| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$$|  $$$$$$$| $$            |  $$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$
		|__/  |__/ \_______/|__/  |__/ \_______/ \_______/|__/             \______/  \______/  \_______/ \_______/



		  /$$$$$$                                      /$$   /$$
		 /$$__  $$                                    | $$  | $$
		| $$  \__/  /$$$$$$   /$$$$$$   /$$$$$$$      | $$  | $$  /$$$$$$   /$$$$$$   /$$$$$$
		| $$ /$$$$ /$$__  $$ /$$__  $$ /$$_____/      | $$$$$$$$ /$$__  $$ /$$__  $$ /$$__  $$
		| $$|_  $$| $$  \ $$| $$$$$$$$|  $$$$$$       | $$__  $$| $$$$$$$$| $$  \__/| $$$$$$$$
		| $$  \ $$| $$  | $$| $$_____/ \____  $$      | $$  | $$| $$_____/| $$      | $$_____/
		|  $$$$$$/|  $$$$$$/|  $$$$$$$ /$$$$$$$/      | $$  | $$|  $$$$$$$| $$      |  $$$$$$$
		 \______/  \______/  \_______/|_______/       |__/  |__/ \_______/|__/       \_______/
*/

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}

	delete game_loader;
}