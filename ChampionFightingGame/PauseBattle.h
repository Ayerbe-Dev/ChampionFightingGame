#pragma once
#include "Scene.h"
#include "GameTexture.h"
#include "GameManager.h"
#include "Player.h"

void pause_battle_main();

class PauseBattle : public Scene {
public:
	PauseBattle();
	~PauseBattle();

	void process_main();
	void render_main();
};
