#pragma once
#include "GameState.h"
#include "GameTexture.h"
#include "GameManager.h"

void title_screen_main();

class TitleScreen: public GameState{
public:
    TitleScreen();
    ~TitleScreen();

    void render();
    void event_any_press();
private:
    GameTexture title_l1;
    GameTexture title_l2;
    GameTexture title_l3;
    GameTexture title_l4;
    GameTexture text;
};