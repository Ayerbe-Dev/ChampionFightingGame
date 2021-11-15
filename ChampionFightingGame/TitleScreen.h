#pragma once
#include "GameState.h"
#include "GameTexture.h"

int title_screen(PlayerInfo player_info[2]);
class TitleScreen{
public:
    TitleScreen();
    void render();
    void start();
    bool titleing = true;
private:
    GameTexture title_l1;
    GameTexture title_l2;
    GameTexture title_l3;
    GameTexture title_l4;
    GameTexture text;
};