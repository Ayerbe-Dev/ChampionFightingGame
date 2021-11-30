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
    void event_start_press();
private:
    GameTexture title_l1;
    GameTexture title_l2;
    GameTexture title_l3;
    GameTexture title_l4;
    GameTexture text;
};