#pragma once
#include <SDL.h>
#include "PlayerInfo.h"

class GameTexture{
public:
    SDL_Rect destRect;
    SDL_Rect srcRect;

    void render();
    bool init(string sTexturePath, SDL_Renderer *pRenderer);
private:
    SDL_Renderer *pRenderer;
    bool bIsInitialized = false;
    SDL_Texture *pTexture;

};

class FixedCharacterSlot{
public:
    bool isInitialized();
    void init(int id, SDL_Renderer *pRenderer);
    void setXPosition(int iX);
    void setYPosition(int iY);
    int getTextureWidth();
    void render();
private:
    GameTexture gameTexture;
    int iCharacterId = -1;
    bool bInitialized = false;
};

class CSSMenu{
public:
    CSSMenu(SDL_Renderer *pRenderer);
    void traverseUp();
    void traverseDown();
    void traverseLeft();
    void traverseRight();
    int getFixedCharacterSlotsLength();
    void render();
private:
    GameTexture backgroundTexture;
    SDL_Renderer *pRenderer;
    void addFixedCharacter(int id, SDL_Renderer *pRenderer);
    PlayerInfo aPlayerInfos[2];
    FixedCharacterSlot aFixedCharacterSlots[32];
};

int cssMenu(SDL_Renderer *pRrenderer, SDL_Window *pWindow, PlayerInfo aPlayerInfo[2]);