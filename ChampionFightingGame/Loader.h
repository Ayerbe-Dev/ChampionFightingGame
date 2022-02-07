#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"
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
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					exit(0);
				}
				break;
			}
		}

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

	return 0;
}

static int LoadGame(void* void_GameLoader) {
	int time = SDL_GetTicks();
	GameLoader* game_loader = (GameLoader*)void_GameLoader;

	game_loader->loaded_items++;

	game_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!game_loader->can_ret) {
	}
	return 1;
}

class BattleLoader : public GameLoader {
public:
	BattleLoader() {};

	Fighter* fighter[2];
	FighterAccessor* fighter_accessor;
	IObject* p1;
	IObject* p2;
	HealthBar health_bar[2];
	ExBar ex_bar[2];
	PlayerIndicator player_indicator[2];
	GameTimer timer;
	Stage stage;
};

static int LoadBattle(void* void_BattleLoader) {
	int time = SDL_GetTicks();
	BattleLoader* battle_loader = (BattleLoader*)void_BattleLoader;
	GameTimer timer;
	Stage stage;
	IObject* p1;
	IObject* p2;
	Fighter* fighter[2];
	HealthBar health_bar[2];
	ExBar ex_bar[2];
	PlayerIndicator player_indicator[2];
	FighterAccessor* fighter_accessor = new FighterAccessor;

	int rng = rand() % 2;
	stage = battle_loader->player_info[rng]->stage; 
	battle_loader->loaded_items++;
	string background_texture = stage.resource_dir + "background.png";
	stage.pBackgroundTexture = loadSDLTexture(background_texture.c_str());
	battle_loader->loaded_items++;
	frameTimeDelay();

	p1 = new IObject(OBJECT_TYPE_FIGHTER, battle_loader->player_info[0]->chara_kind, 0, battle_loader->player_info[0], fighter_accessor);
	battle_loader->loaded_items++;
	frameTimeDelay();

	p2 = new IObject(OBJECT_TYPE_FIGHTER, battle_loader->player_info[1]->chara_kind, 1, battle_loader->player_info[1], fighter_accessor);
	battle_loader->loaded_items++;
	frameTimeDelay();

	fighter[0] = p1->get_fighter();
	battle_loader->loaded_items++;
	frameTimeDelay();

	fighter[1] = p2->get_fighter();
	battle_loader->loaded_items++;
	frameTimeDelay();

	for (int i = 0; i < 2; i++) {
		fighter[i]->player_info = battle_loader->player_info[i];
		fighter[i]->pos.x = 0;
		fighter_accessor->fighter[i] = fighter[i];
		fighter[i]->fighter_accessor = fighter_accessor;
		battle_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		fighter[i]->superInit(i);
		battle_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			fighter[i]->projectiles[i2]->owner_id = i;
		}
		battle_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		health_bar[i] = HealthBar(fighter[i]);
		battle_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		ex_bar[i] = ExBar(fighter[i]);
		battle_loader->loaded_items++;
		frameTimeDelay();
	}

	timer = GameTimer(99);
	battle_loader->loaded_items++;
	frameTimeDelay();

	for (int i = 0; i < 2; i++) {
		player_indicator[i] = PlayerIndicator(fighter[i]);
	}

	g_soundmanager.fighter_accessor = fighter_accessor;

	if (getGameSetting("music_setting") == MUSIC_SETTING_STAGE) {
		g_soundmanager.loadMusic(stage.default_music_kind);
	}
	else if (getGameSetting("music_setting") == MUSIC_SETTING_CHARA) {
		//randomly play the theme of one of the characters. if online, always play the opponent's theme
	}
	else {
		//randomly play the theme for one of the players' tags. if online, always play the user's theme
	}

	battle_loader->stage = stage;
	battle_loader->fighter_accessor = fighter_accessor;
	battle_loader->p1 = p1;
	battle_loader->p2 = p2;
	for (int i = 0; i < 2; i++) {
		battle_loader->fighter[i] = fighter[i];
		battle_loader->fighter[i]->loadCharaSounds();
		battle_loader->player_indicator[i] = player_indicator[i];
		battle_loader->health_bar[i] = health_bar[i];
		battle_loader->ex_bar[i] = ex_bar[i];
	}
	battle_loader->timer = timer;
	battle_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!battle_loader->can_ret) {
	}
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