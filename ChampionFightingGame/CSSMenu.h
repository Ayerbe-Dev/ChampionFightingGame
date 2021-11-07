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
    void querryFixedCssSlotPosition(int indexLocation, int *xptr, int *yptr);

    int aPlayerSelectionIndex[2] = {0,0};
private:
   
    GameTexture backgroundTexture;
    GameTexture bigBarTexture;
    GameTexture topBarTexture;
    SDL_Renderer *pRenderer;
    void addFixedCharacter(int id);
    PlayerInfo aPlayerInfos[2];
    FixedCharacterSlot aFixedCharacterSlots[32];
};

class CssCursor{
public:
    void init(string sTexturePath);
    void setTarget(int x, int y);
    void render();
    GameTexture cursorTexture;
    int iXTarget = 0;
    int iYTarget = 0;
private:
    float partialX=0;
    float partialY=0;
};

int cssMenu(PlayerInfo aPlayerInfo[2]);