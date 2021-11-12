#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"

extern SDL_mutex* mutex;

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
	int loaded_items = 0;
	bool finished = false;
	bool can_ret = false;
	Stage stage;

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
	SDL_LockMutex(mutex);	
		stage = Stage(game_loader->player_info[rng].stage_kind); 
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	SDL_LockMutex(mutex);
		p1 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[0])->chara_kind, 0, &game_loader->player_info[0], fighter_accessor);
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	SDL_LockMutex(mutex);
		p2 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[1])->chara_kind, 1, &game_loader->player_info[1], fighter_accessor);
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	SDL_LockMutex(mutex);
		fighter[0] = p1->get_fighter();
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	SDL_LockMutex(mutex);
		fighter[1] = p2->get_fighter();
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	for (int i = 0; i < 2; i++) {
		SDL_LockMutex(mutex);
			fighter[i]->player_info = &game_loader->player_info[i];
			fighter[i]->pos.x = 0;
			fighter_accessor->fighter[i] = fighter[i];
			fighter[i]->fighter_accessor = fighter_accessor;
			game_loader->loaded_items++;
		SDL_UnlockMutex(mutex);
	}
	for (int i = 0; i < 2; i++) {
		SDL_LockMutex(mutex);
			fighter[i]->superInit(i);
			game_loader->loaded_items++;
		SDL_UnlockMutex(mutex);
	}
	for (int i = 0; i < 2; i++) {
		SDL_LockMutex(mutex);
			for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
				fighter[i]->projectiles[i2]->owner_id = i;
			}
			game_loader->loaded_items++;
		SDL_UnlockMutex(mutex);
	}
	for (int i = 0; i < 2; i++) {
		SDL_LockMutex(mutex);
		cout << fighter[i]->chara_kind << endl;
			health_bar[i] = HealthBar(fighter[i]);
			game_loader->loaded_items++;
		SDL_UnlockMutex(mutex);
	}

	SDL_LockMutex(mutex);
		timer = GameTimer(99);
		game_loader->loaded_items++;
	SDL_UnlockMutex(mutex);

	for (int i = 0; i < 2; i++) {
		SDL_LockMutex(mutex);
			player_indicator[i] = PlayerIndicator(fighter[i]);
		SDL_UnlockMutex(mutex);
	}

	game_loader->stage = stage;
	game_loader->fighter_accessor = fighter_accessor;
	game_loader->p1 = p1;
	game_loader->p2 = p2;
	for (int i = 0; i < 2; i++) {
		game_loader->fighter[i] = fighter[i];
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