#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"

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
	int loaded_items = 0;
	bool finished = false;
	bool can_ret = false;
};

static int LoadGame(void* void_loader) {
	return 0;
}

class BattleLoader : public GameLoader {
public:
	BattleLoader();
	BattleLoader(PlayerInfo p1, PlayerInfo p2);

	Fighter* fighter[2];
	FighterAccessor* fighter_accessor;
	IObject* p1;
	IObject* p2;
	HealthBar health_bar[2];
	ExBar ex_bar[2];
	PlayerIndicator player_indicator[2];
	GameTimer timer;
	Stage stage;

	PlayerInfo player_info[2];
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