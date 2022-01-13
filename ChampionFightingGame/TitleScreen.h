#pragma once
#include "GameState.h"
#include "GameTexture.h"
#include "GameMenu.h"
#include "GameManager.h"

void title_screen_main(GameManager *game_manager);

class TitleScreen: public GameMenu{
public:
    TitleScreen();
    void render();
    void event_any_press();
private:
    GameTextureNew title_l1;
    GameTextureNew title_l2;
    GameTextureNew title_l3;
    GameTextureNew title_l4;
    GameTextureNew text;
};