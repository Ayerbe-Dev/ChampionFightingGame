#pragma once
#include <SDL.h>
#include "PlayerInfo.h"
#include "GameTexture.h"

class FixedCharacterSlot{
public:
    GameTexture gameTexture;
    bool isInitialized();
    void init(int id);
    void setXPosition(int iX);
    void setYPosition(int iY);
    int getTextureWidth();
    void render();
private:
    int iCharacterId = -1;
    bool bInitialized = false;
};

class CSSMenu{
public:
    CSSMenu();
    void traverseUp();
    void traverseDown();
    void traverseLeft();
    void traverseRight();
    int getSlotsLength();
    void render();
private:
    GameTexture backgroundTexture;
    GameTexture bigBarTexture;
    GameTexture topBarTexture;
    SDL_Renderer *pRenderer;
    void addFixedCharacter(int id);
    PlayerInfo aPlayerInfos[2];
    FixedCharacterSlot aFixedCharacterSlots[32];
};

int cssMenu(PlayerInfo aPlayerInfo[2]);