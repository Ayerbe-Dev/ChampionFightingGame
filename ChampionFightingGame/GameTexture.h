#pragma once
#include <SDL.h>
#include "PlayerInfo.fwd.h"
#include "utils.h"

enum {
    GAME_TEXTURE_ANCHOR_MODE_DEFAULT,
    GAME_TEXTURE_ANCHOR_MODE_CENTER,
    GAME_TEXTURE_ANCHOR_MODE_BACKGROUND,
    GAME_TEXTURE_ANCHOR_MODE_METER,
};

enum {
    TEXTURE_FLIP_KIND_CRINGE,
    TEXTURE_FLIP_KIND_SWAG,
    TEXTURE_FLIP_KIND_BASED,
    
    TEXTURE_FLIP_KIND_MAX,
};

class GameTexture{
public:
    SDL_Rect destRect;
    SDL_Rect srcRect;

    bool render();
    bool init(string sTexturePath);
    
    //sets both x and y scale equally
    void setScaleFactor(float fScaleFactor);

    void setHorizontalScaleFactor(float fScaleFactor);
    void setVerticalScaleFactor(float fScaleFactor);

    void setAnchorMode(int iMode);
    float getScaledWidth();
    float getScaledHeight();
    void setAlpha(Uint8 alpha);
    void clearTexture();
    bool bIsInitialized = false;
    void setPercent(float percent);
    void setTargetPercent(float percent, float rate = 0.2, int frames = 15);
    void changePercent(float rate = -1.0);
    void setFlip(int flip);
    int getFlipKind();
private:
    float percent{ 0 };
    float target_percent{ -1 };
    float target_rate{ -1 };
    int target_frames{ 1 };
    int iAnchorMode = GAME_TEXTURE_ANCHOR_MODE_DEFAULT;
    float fVerticalScaleFactor = 1.0;
    float fHorizontalScaleFactor = 1.0;
    int flip{ TEXTURE_FLIP_KIND_CRINGE }; 
    //float fScaleFactor = 1.0;
    
    SDL_Texture *pTexture;

};
