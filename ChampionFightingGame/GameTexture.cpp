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

bool GameTexture::render() {
    if (!bIsInitialized) {
        return false;
    }

    if (target_percent != -1.0) {
        changePercent();
    }

    SDL_Rect tmpDestRect = destRect;
    tmpDestRect.w *= fHorizontalScaleFactor;
    tmpDestRect.h *= fVerticalScaleFactor;

    switch (iAnchorMode) {
        case GAME_TEXTURE_ANCHOR_MODE_CENTER:
            tmpDestRect.x -= tmpDestRect.w / 2;
            tmpDestRect.y -= tmpDestRect.h / 2;
            SDL_RenderCopy(g_renderer, pTexture, nullptr, &tmpDestRect);
            break;
        case GAME_TEXTURE_ANCHOR_MODE_BACKGROUND:
            SDL_RenderCopy(g_renderer, pTexture, nullptr, nullptr);
            break;
        case GAME_TEXTURE_ANCHOR_MODE_METER:
            tmpDestRect.w = destRect.w * (percent);
            srcRect.w = tmpDestRect.w;
            SDL_RenderCopy(g_renderer, pTexture, &srcRect, &tmpDestRect);
            break;
        default:
            SDL_RenderCopy(g_renderer, pTexture, nullptr, &tmpDestRect);
            break;
    }
    return true;
}

/*bool GameTexture::render() {
    if (!bIsInitialized){
        return false;
    }

    if (target_percent != -1.0) {
        changePercent();
    }

    SDL_Rect* tmpDestRect = nullptr;
    SDL_Rect* tmpSrcRect = nullptr;


    switch (iAnchorMode) {
        case (GAME_TEXTURE_ANCHOR_MODE_CENTER): {
            tmpDestRect = &destRect;
            tmpDestRect->w *= fHorizontalScaleFactor;
            tmpDestRect->h *= fVerticalScaleFactor;
            tmpDestRect->x -= tmpDestRect->w / 2;
            tmpDestRect->y -= tmpDestRect->h / 2;
        }
        break;
        case (GAME_TEXTURE_ANCHOR_MODE_METER): {
            tmpDestRect = &destRect;
            tmpDestRect->w = destRect.w * (percent);
            tmpDestRect->h *= fVerticalScaleFactor;
            tmpSrcRect = &srcRect;
            tmpSrcRect->w = tmpDestRect->w;
        }
        break;
        case (GAME_TEXTURE_ANCHOR_MODE_BACKGROUND): {} break;
        default: {
            tmpDestRect = &destRect;
        }
        break;
    }
    if (tmpSrcRect == nullptr && tmpDestRect != nullptr) {
        cout << "Yeet" << endl;
    }
    if (tmpDestRect == nullptr && tmpSrcRect != nullptr) {
        cout << "Yort" << endl;
    }
    SDL_RenderCopyEx(g_renderer, pTexture, tmpSrcRect, tmpDestRect, 0, NULL, flip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);

    return true;
}*/

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

void GameTexture::setTargetPercent(float percent, float rate, int frames) {
    target_percent = percent;
    target_rate = rate;
    target_frames = frames;
    if (iAnchorMode != GAME_TEXTURE_ANCHOR_MODE_METER) {
        printf("WARNING: GameTexture is using the setTargetPercent() function but its not in the correct mode!\n");
    }
}

void GameTexture::changePercent(float rate) {
    if (rate == -1.0) {
        rate = target_rate;
    }
    if (rate == -1.0) {
        cout << "WARNING: Target rate was not set through setTargetPercent, but rate was not given a non-default arg!" << endl;
    }

    if (target_percent != percent) {
        if (target_percent < percent) {
            percent = clampf(target_percent, percent - (rate / target_frames), percent);
        }
        else {
            percent = clampf(percent, percent + (rate / target_frames), target_percent);
        }
    }
    else {
        target_percent = -1.0;
        target_rate = -1.0;
    }
}

void GameTexture::setFlip(bool flip) {
    this->flip = flip;
}

bool GameTexture::isFlip() {
    return flip;
}