#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"
#include "CharaSelect.h"
#include "DebugMenu.h"
#include "Menu.h"

extern SDL_mutex* mutex;
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
	PlayerInfo player_info[2];

	int loaded_items = 0;
	bool finished = false;
	bool can_ret = false;
};

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
	stage = battle_loader->player_info[rng].stage; 
	battle_loader->loaded_items++;
	string background_texture = stage.resource_dir + "background.png";
	stage.pBackgroundTexture = loadTexture(background_texture.c_str());
	battle_loader->loaded_items++;
	frameTimeDelay();

	p1 = new IObject(OBJECT_TYPE_FIGHTER, (&battle_loader->player_info[0])->chara_kind, 0, &battle_loader->player_info[0], fighter_accessor);
	battle_loader->loaded_items++;
	frameTimeDelay();

	p2 = new IObject(OBJECT_TYPE_FIGHTER, (&battle_loader->player_info[1])->chara_kind, 1, &battle_loader->player_info[1], fighter_accessor);
	battle_loader->loaded_items++;
	frameTimeDelay();

	fighter[0] = p1->get_fighter();
	battle_loader->loaded_items++;
	frameTimeDelay();

	fighter[1] = p2->get_fighter();
	battle_loader->loaded_items++;
	frameTimeDelay();

	for (int i = 0; i < 2; i++) {
		fighter[i]->player_info = &battle_loader->player_info[i];
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
	return 1; 
}

class CharaSelectLoader : public GameLoader {
public:
	CharaSelectLoader() {};

	CSS css;
	CssCursor css_cursor[2];
};

static int LoadCharaSelect(void* void_CharaSelectLoader) {
	int time = SDL_GetTicks();
	CharaSelectLoader* chara_select_loader = (CharaSelectLoader*)void_CharaSelectLoader;

	CSS css;
	CssCursor cursors[2];

	css.player_info[0] = &chara_select_loader->player_info[0];
	css.player_info[1] = &chara_select_loader->player_info[1];

	if (css.loadCSS()) {
		displayLoadingScreen();
		chara_select_loader->player_info[0].crash_reason = "Could not open CSS file!";
		return 0;
	}
	chara_select_loader->loaded_items++;
	if (css.numRows == 0) {
		css.myCol[0] = 1;
		css.myCol[1] = 1;
	}
	for (int i = 0; i < 2; i++) {
		css.player_id = i;
		if (css.player_info[i]->chara_kind != CHARA_KIND_MAX) {
			css.findPrevChara(css.player_info[i]->chara_kind);
		}
	}

	cursors[0].init("resource/ui/menu/css/p1Cursor.png");
	cursors[0].cursorTexture.setScaleFactor(1.2);
	cursors[0].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
	chara_select_loader->loaded_items++;

	cursors[1].init("resource/ui/menu/css/p2Cursor.png");
	cursors[1].cursorTexture.setScaleFactor(1.2);
	cursors[1].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
	chara_select_loader->loaded_items++;

	chara_select_loader->css = css;
	chara_select_loader->css_cursor[0] = cursors[0];
	chara_select_loader->css_cursor[1] = cursors[1];

	chara_select_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!chara_select_loader->can_ret) {
	}
	return 1;
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
	debug_list.addEntry("Debug (this menu)", DEBUG_LIST_SELECTABLE, GAME_STATE_DEBUG_MENU);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry("Close", DEBUG_LIST_SELECTABLE, GAME_STATE_CLOSE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry(debug_loader->player_info[0].crash_reason, DEBUG_LIST_NOT_SELECTABLE);
	debug_loader->loaded_items++;
	frameTimeDelay();
	debug_list.addEntry(debug_loader->player_info[1].crash_reason, DEBUG_LIST_NOT_SELECTABLE);
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

class MenuLoader : public GameLoader {
public:
	MenuLoader() {};

	MainMenu main_menu;
	SDL_Texture* bgTexture;
	MenuItem menu_items[5];
	SubMenuTable* sub_menu_tables[5];
};

static int LoadMenu(void* void_MenuLoader) {
	int time = SDL_GetTicks();
	MenuLoader* menu_loader = (MenuLoader*)void_MenuLoader;

	MainMenu main_menu;
	SDL_Texture* bgTexture;
	MenuItem menu_items[5];
	SubMenuTable* sub_menu_tables[5];

	bgTexture = loadTexture("resource/ui/menu/main/bg.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	menu_items[0] = MenuItem("resource/ui/menu/main/Online.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	menu_items[1] = MenuItem{ "resource/ui/menu/main/Solo.png" };
	menu_loader->loaded_items++;
	frameTimeDelay();
	menu_items[2] = MenuItem{ "resource/ui/menu/main/VS.png", "resource/ui/menu/main/vsimg.png" };
	menu_loader->loaded_items++;
	frameTimeDelay();
	menu_items[3] = MenuItem{ "resource/ui/menu/main/Options.png" };
	menu_loader->loaded_items++;
	frameTimeDelay();
	menu_items[4] = MenuItem{ "resource/ui/menu/main/Extras.png" };
	menu_loader->loaded_items++;
	frameTimeDelay();
	for (int i = 0; i < 5; i++) {
		menu_items[i].destination = i;
		sub_menu_tables[i] = new SubMenuTable(i);
		menu_loader->loaded_items++;
		frameTimeDelay();
	}
	sub_menu_tables[SUB_MENU_ONLINE]->item_count = 2;
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_ONLINE]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->item_count = 3;
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Story.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Arcade.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_SINGLEPLAYER]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Training.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_VS]->item_count = 3;
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/PlayerVsPlayer.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/PlayerVsCPU.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_VS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_OPTIONS]->item_count = 5;
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_OPTIONS]->sub_option_text[4] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_EXTRAS]->item_count = 4;
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[0] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[1] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[2] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();
	sub_menu_tables[SUB_MENU_EXTRAS]->sub_option_text[3] = loadTexture("resource/ui/menu/main/Placeholder.png");
	menu_loader->loaded_items++;
	frameTimeDelay();

	menu_loader->main_menu = main_menu;
	menu_loader->bgTexture = bgTexture;
	for (int i = 0; i < 5; i++) {
		menu_loader->menu_items[i] = menu_items[i];
		menu_loader->sub_menu_tables[i] = sub_menu_tables[i];
		menu_loader->loaded_items+=2;
		frameTimeDelay();
	}
	
	menu_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!menu_loader->can_ret) {
	}
	return 1;
}