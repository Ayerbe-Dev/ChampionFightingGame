#pragma once
#include <SDL.h>
#include "PlayerInfo.h"
#include "utils.h"
class HealthBar
{
public:
    float fMaxhealth;
    PlayerInfo *pPlayerLink;

    SDL_Texture *pForegroundTexture;
    SDL_Texture *pBackgroundTexture;
    SDL_Rect cForegroundRect{0, 0, 400, 50};
    SDL_Rect cBackgroundRect{0, 0, 400, 50};

    HealthBar();
    HealthBar(float fMaxHealth, PlayerInfo *pPlayerLink, SDL_Renderer *pRenderer);
    void Render(SDL_Renderer *pRenderer);
};