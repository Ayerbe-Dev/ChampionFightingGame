#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "CharaSelect.h"
#include "StageSelect.h"
#include "DebugMenu.h"
#include "Menu.h"

extern SDL_mutex* file_mutex;
extern SDL_Renderer* g_renderer;
extern SoundManager g_soundmanager;

class LoadIcon {
public:
	LoadIcon();

	GameTexture texture;
	bool move_right;
	bool move_down;
	bool panic_v;
	bool panic_mode;
	float panic_speed;
	float panic_add;
	bool panic_setting; //True = Gradually slows down to a stop, False = Goes flying

	void move();
	void set_attributes();
	bool check_corner_distance(bool init);
};

class GameLoader {
public:
	GameLoader();
	GameLoader(int total_items);

	PlayerInfo *player_info[2];

	int loaded_items = 0;
	int total_items;
	int ret = 0;
	bool finished = false;
	bool can_ret = false;
};

static int LoadingScreen(void* void_GameLoader) {
	GameLoader* game_loader = (GameLoader*)void_GameLoader;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (!game_loader->finished) {
		frameTimeDelay();

		load_icon.move();
		SDL_LockMutex(file_mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		loadingBar.setTargetPercent(((float)game_loader->loaded_items / (float)game_loader->total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(file_mutex);
	}

	loadingSplash.clearTexture();
	loadingBar.clearTexture();
	loadingFlavor.clearTexture();
	load_icon.texture.clearTexture();
	SDL_DestroyTexture(pScreenTexture);

	return 0;
}

class DebugLoader : public GameLoader {
public:
	DebugLoader() {};

	std::ostringstream lastString;
	TTF_Font* debug_font;
	debug_list debug_list;
};

static int LoadDebug(void* void_DebugLoader) {
	int time = SDL_GetTicks();
	DebugLoader* debug_loader = (DebugLoader*)void_DebugLoader;

	TTF_Font* debug_font = loadDebugFont();
	debug_loader->loaded_items++;
	debug_list debug_list = { debug_font };
	debug_loader->loaded_items++;

	debug_list.addEntry("Welcome to the debug menu!", DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Use 'SPACE' or 'ENTER' to select an option.", DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry(debug_loader->lastString.str(), DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Title Screen", DEBUG_LIST_SELECTABLE, GAME_STATE_TITLE_SCREEN);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Menu", DEBUG_LIST_SELECTABLE, GAME_STATE_MENU);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Game", DEBUG_LIST_SELECTABLE, GAME_STATE_BATTLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("CSS", DEBUG_LIST_SELECTABLE, GAME_STATE_CHARA_SELECT);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("3D Test", DEBUG_LIST_SELECTABLE, GAME_STATE_3D);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Debug (this menu)", DEBUG_LIST_SELECTABLE, GAME_STATE_DEBUG_MENU);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Close", DEBUG_LIST_SELECTABLE, GAME_STATE_CLOSE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry(debug_loader->player_info[0]->crash_reason, DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry(debug_loader->player_info[1]->crash_reason, DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();

	debug_list.event_down_press();
	debug_loader->loaded_items++;
	frameTimeDelay();

	debug_loader->debug_list = debug_list;
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_loader->debug_font = debug_font;
	debug_loader->loaded_items++;
	frameTimeDelay();

	debug_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!debug_loader->can_ret) {
	}
	return 1;
}

class StageSelectLoader : public GameLoader {
public:
	StageSelectLoader() {};

	StageSelect stage_select;
};

static int LoadStageSelect(void* void_StageSelectLoader) {
	int time = SDL_GetTicks();
	GameLoader* stage_select_loader = (GameLoader*)void_StageSelectLoader;

	StageSelect stage_select;

	stage_select.player_info[0] = stage_select_loader->player_info[0];
	stage_select.player_info[1] = stage_select_loader->player_info[1];

	if (stage_select.load_stage_select()) {
		displayLoadingScreen();
		stage_select_loader->player_info[0]->crash_reason = "Could not open Stage Select file!";
		return 1;
	}
	stage_select_loader->loaded_items++;
	frameTimeDelay();

	stage_select_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!stage_select_loader->can_ret) {}
	return 0;
}