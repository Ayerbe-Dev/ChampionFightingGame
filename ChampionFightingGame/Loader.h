#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"

extern SDL_mutex* mutex;
extern SoundManager g_soundmanager;

class GameLoader {
public:
	GameLoader();
	GameLoader(PlayerInfo p1, PlayerInfo p2);

	Fighter* fighter[2];
	FighterAccessor* fighter_accessor;
	IObject* p1;
	IObject* p2;
	HealthBar health_bar[2];
	PlayerIndicator player_indicator[2];
	GameTimer timer;
	Stage stage;
	int loaded_items = 0;
	bool finished = false;
	bool can_ret = false;

	PlayerInfo player_info[2];
};

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

static int LoadGame(void* void_gameloader) {
	int time = SDL_GetTicks();
	GameLoader* game_loader = (GameLoader*)void_gameloader;
	GameTimer timer;
	Stage stage;
	IObject* p1;
	IObject* p2;
	Fighter* fighter[2];
	HealthBar health_bar[2];
	PlayerIndicator player_indicator[2];
	FighterAccessor* fighter_accessor = new FighterAccessor;

	int rng = rand() % 2;
	stage = game_loader->player_info[rng].stage; 
	game_loader->loaded_items++;
	string background_texture = stage.resource_dir + "background.png";
	stage.pBackgroundTexture = loadTexture(background_texture.c_str());
	game_loader->loaded_items++;
	frameTimeDelay();

	p1 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[0])->chara_kind, 0, &game_loader->player_info[0], fighter_accessor);
	game_loader->loaded_items++;
	frameTimeDelay();

	p2 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[1])->chara_kind, 1, &game_loader->player_info[1], fighter_accessor);
	game_loader->loaded_items++;
	frameTimeDelay();

	fighter[0] = p1->get_fighter();
	game_loader->loaded_items++;
	frameTimeDelay();

	fighter[1] = p2->get_fighter();
	game_loader->loaded_items++;
	frameTimeDelay();

	for (int i = 0; i < 2; i++) {
		fighter[i]->player_info = &game_loader->player_info[i];
		fighter[i]->pos.x = 0;
		fighter_accessor->fighter[i] = fighter[i];
		fighter[i]->fighter_accessor = fighter_accessor;
		game_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		fighter[i]->superInit(i);
		game_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			fighter[i]->projectiles[i2]->owner_id = i;
		}
		game_loader->loaded_items++;
		frameTimeDelay();
	}

	for (int i = 0; i < 2; i++) {
		health_bar[i] = HealthBar(fighter[i]);
		game_loader->loaded_items++;
		frameTimeDelay();
	}

	timer = GameTimer(99);
	game_loader->loaded_items++;
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

	game_loader->stage = stage;
	game_loader->fighter_accessor = fighter_accessor;
	game_loader->p1 = p1;
	game_loader->p2 = p2;
	for (int i = 0; i < 2; i++) {
		game_loader->fighter[i] = fighter[i];
		game_loader->fighter[i]->loadCharaSounds();
		game_loader->player_indicator[i] = player_indicator[i];
		game_loader->health_bar[i] = health_bar[i];
	}
	game_loader->timer = timer;
	game_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	while (!game_loader->can_ret) {
	}
	return 1; 
} 