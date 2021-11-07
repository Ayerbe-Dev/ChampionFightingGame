#include "GameTexture.h"
extern SDL_Renderer* g_renderer;

bool GameTexture::init(string sTexturePath){
    if (bIsInitialized){
        printf("GameTexture already initialized!\n");
        return false;
    }
    //this->pRenderer = pRenderer;
    pTexture = loadTexture(sTexturePath.c_str());
    SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);
    destRect.x=0;
    destRect.y=0;
}

void GameTexture::render(){
    SDL_Rect tmpDestRect = destRect;
    tmpDestRect.w *= fScaleFactor;
    tmpDestRect.h *= fScaleFactor;

    switch (iAnchorMode)
    {
    case GAME_TEXTURE_ANCHOR_MODE_CENTER:
        tmpDestRect.x -= tmpDestRect.w/2;
        tmpDestRect.y -= tmpDestRect.h/2;
        SDL_RenderCopy(g_renderer,pTexture,nullptr,&tmpDestRect);
        break;
    case GAME_TEXTURE_ANCHOR_MODE_BACKGROUND:
        SDL_RenderCopy(g_renderer,pTexture,nullptr,nullptr);
        break;
    default:
        SDL_RenderCopy(g_renderer,pTexture,nullptr,&tmpDestRect);
        break;
    }
    //printf("Width: %d, Height: %d\n", tmpDestRect.w, tmpDestRect.h);
}

void GameTexture::setScaleFactor(float fScaleFactor){
    this->fScaleFactor = fScaleFactor;
}

float GameTexture::getScaledWidth(){
    return destRect.w * fScaleFactor;
}

float GameTexture::getScaledHeight(){
    return destRect.h * fScaleFactor;
}

void GameTexture::setAnchorMode(int iMode){
    iAnchorMode = iMode;
}