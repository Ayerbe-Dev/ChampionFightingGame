#include "GameTexture.h"
extern SDL_Renderer* g_renderer;

bool GameTexture::init(string sTexturePath){
    if (bIsInitialized){
        printf("GameTexture already initialized!\n");
        return false;
    }

    bIsInitialized = true;
    pTexture = loadTexture(sTexturePath.c_str());
    SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);
    destRect.x=0;
    destRect.y=0;

    srcRect.x=0;
    srcRect.y=0;
    srcRect.h=destRect.h;
    srcRect.w=destRect.w;
}

bool GameTexture::render(){
    if (!bIsInitialized){
        return false;
    }

    SDL_Rect tmpDestRect = destRect;
    tmpDestRect.w *= fHorizontalScaleFactor;
    tmpDestRect.h *= fVerticalScaleFactor;

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
    case GAME_TEXTURE_ANCHOR_MODE_METER:
        tmpDestRect.w = destRect.w * (percent);
        srcRect.w = tmpDestRect.w;
        SDL_RenderCopy(g_renderer,pTexture,&srcRect,&tmpDestRect);
        break;
    default:
        SDL_RenderCopy(g_renderer,pTexture,nullptr,&tmpDestRect);
        break;
    }
    return true;
}

void GameTexture::setScaleFactor(float fScaleFactor){
    this->fVerticalScaleFactor = fScaleFactor;
    this->fHorizontalScaleFactor = fScaleFactor;
}

void GameTexture::setHorizontalScaleFactor(float fScaleFactor){
    this->fHorizontalScaleFactor = fScaleFactor;
}

void GameTexture::setVerticalScaleFactor(float fScaleFactor){
    this->fVerticalScaleFactor = fScaleFactor;
}

void GameTexture::clearTexture() {
    SDL_DestroyTexture(pTexture);
}

float GameTexture::getScaledWidth(){
    return destRect.w * fHorizontalScaleFactor;
}

float GameTexture::getScaledHeight(){
    return destRect.h * fVerticalScaleFactor;
}

void GameTexture::setAnchorMode(int iMode){
    iAnchorMode = iMode;
}

void GameTexture::setAlpha(Uint8 alpha){
    SDL_SetTextureAlphaMod(pTexture,alpha);
}

void GameTexture::setPercent(float percent){
    this->percent = percent;
    if (iAnchorMode != GAME_TEXTURE_ANCHOR_MODE_METER){
        printf("WARNING: GameTexture is using the setPercent() function but its not in the correct mode!\n");
    }
}