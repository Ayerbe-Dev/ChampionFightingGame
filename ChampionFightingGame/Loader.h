#pragma once

#include "Fighter.h"
#include "Stage.h"
#include "FighterAccessor.h"
#include "UI.h"

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
	int loaded_items = 0;
	bool finished = false;
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
	//init stage
	int rng = rand() % 2;
	Stage stage = Stage(game_loader->player_info[rng].stage_kind);
	game_loader->loaded_items++;

	//init players
	Fighter* fighter[2];
	game_loader->loaded_items += 2;
	FighterAccessor* fighter_accessor = new FighterAccessor;
	game_loader->loaded_items++;

	IObject* p1 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[0])->chara_kind, 0, &game_loader->player_info[0], fighter_accessor);
	game_loader->loaded_items++;
	IObject* p2 = new IObject(OBJECT_TYPE_FIGHTER, (&game_loader->player_info[1])->chara_kind, 1, &game_loader->player_info[1], fighter_accessor);
	game_loader->loaded_items++;

	fighter[0] = p1->get_fighter();
	game_loader->loaded_items++;
	fighter[1] = p2->get_fighter();
	game_loader->loaded_items++;

	PlayerInfo* player_info = &game_loader->player_info[0];

	for (int i = 0; i < 2; i++) {
		fighter[i]->player_info = &game_loader->player_info[i];
		fighter[i]->pos.x = 0;
		fighter_accessor->fighter[i] = fighter[i];
		fighter[i]->fighter_accessor = fighter_accessor;
		game_loader->loaded_items++;
	}
	for (int i = 0; i < 2; i++) {
		fighter[i]->superInit(i);
		game_loader->loaded_items++;
	}

	for (int i = 0; i < 2; i++) {
		for (int i2 = 0; i2 < MAX_PROJECTILES; i2++) {
			fighter[i]->projectiles[i2]->owner_id = i;
		}
		game_loader->loaded_items++;
	}

	HealthBar health_bar[2];
	health_bar[0] = HealthBar(fighter[0]);
	health_bar[1] = HealthBar(fighter[1]);
	game_loader->loaded_items += 2;

	PlayerIndicator player_indicator[2];
	player_indicator[0] = PlayerIndicator(fighter[0]);
	player_indicator[1] = PlayerIndicator(fighter[1]);
	game_loader->loaded_items += 2;

	game_loader->stage = stage;
	game_loader->fighter_accessor = fighter_accessor;
	game_loader->p1 = p1;
	game_loader->p2 = p2;
	for (int i = 0; i < 2; i++) {
		game_loader->fighter[i] = fighter[i];
		game_loader->player_indicator[i] = player_indicator[i];
		game_loader->health_bar[i] = health_bar[i];
	}
	game_loader->finished = true;
	cout << "This thread was active for " << SDL_GetTicks() - time << " ms" << endl;
	return 1; 
}