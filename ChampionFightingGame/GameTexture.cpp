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
    default:
        //printf("Default Render mode\n");
        SDL_RenderCopy(g_renderer,pTexture,nullptr,&tmpDestRect);
        break;
    }
    //printf("Width: %d, Height: %d\n", tmpDestRect.w, tmpDestRect.h);
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

void GameTexture::setAlpha(int alpha) {
    SDL_SetTextureAlphaMod(pTexture, alpha);
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