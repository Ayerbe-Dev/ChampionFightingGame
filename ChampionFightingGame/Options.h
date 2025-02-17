#pragma once
#include "Scene.h"
#include "Player.h"

void controls_main();

class OptionsMenu : public Scene {
public:
	OptionsMenu();
	~OptionsMenu();
	void render_main();
};