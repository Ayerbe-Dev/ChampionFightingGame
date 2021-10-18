#include "UI.h"
#include <SDL.h>

HealthBar::HealthBar(float fMaxHealth, PlayerInfo *pPlayerLink, SDL_Renderer *pRenderer)
{
    this->fMaxhealth = fMaxHealth;
    this->pPlayerLink = pPlayerLink;

    SDL_Rect cTmpRect{0, 0, 400, 50};
    this->cForegroundRect = cTmpRect;
    this->cBackgroundRect = cTmpRect;

    pBackgroundTexture = loadTexture("resource/game/BG.png", pRenderer);
    pForegroundTexture = loadTexture("resource/game/FG.png", pRenderer);
}

void HealthBar::Render(SDL_Renderer *pRenderer)
{
    SDL_RenderCopy(pRenderer, pBackgroundTexture, nullptr, &cBackgroundRect);
    SDL_Rect cTmpRect{0, 0, pPlayerLink->chara_float[CHARA_FLOAT_HEALTH] / fMaxhealth * 400, 50};
    cForegroundRect = SDL_Rect{0, 0, cTmpRect.w, 50};
    SDL_RenderCopy(pRenderer, pForegroundTexture, &cTmpRect, &cForegroundRect);
}