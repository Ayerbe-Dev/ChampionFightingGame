#pragma once
#include <SDL.h>
#include "PlayerInfo.h"

enum{
    GAME_TEXTURE_ANCHOR_MODE_DEFAULT,
    GAME_TEXTURE_ANCHOR_MODE_CENTER,
    GAME_TEXTURE_ANCHOR_MODE_BACKGROUND
};

class GameTexture{
public:
    SDL_Rect destRect;
    SDL_Rect srcRect;

    void render();
    bool init(string sTexturePath);
    void setScaleFactor(float fScaleFactor);
    void setAnchorMode(int iMode);
    float getScaledWidth();
    float getScaledHeight();
    
private:
    int iAnchorMode = GAME_TEXTURE_ANCHOR_MODE_DEFAULT;
    float fScaleFactor = 1.0;
    //SDL_Renderer *pRenderer;
    bool bIsInitialized = false;
    SDL_Texture *pTexture;

};
