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

    bool isBelow(int y);
    bool isAbove(int y);
private:
    int iCharacterId = -1;
    bool bInitialized = false;
};

class MobileCharacterSlot{
public:
    GameTexture gameTexture;
    void playAnim();

    /*
        Sets the glide target
        \param x the x target
        \param y the y target
        \param w the starting width
        \param h the starting height
    */
    void setTarget(int x, int y, float w, float h);
private:
    int iAnimationSpeed=16;
    int targetX=0,targetY=0;
    float fPosX=0,fPosY=0;
    float fTheta=0;
    int iAnimTime=0;
    float fScale=.2;

    float fHorizontalscaleOffset=0,fVerticalscale=0;
};

class CSSMenu{
public:
    CSSMenu();
    void traverseUp();
    void traverseDown();
    void traverseLeft();
    void traverseRight();
    void select();
    void back();
    int getSlotsLength();
    void render();
    void querryFixedCssSlotPosition(int indexLocation, int *xptr, int *yptr);

    int aPlayerSelectionIndex[2] = {0,0};

    int iLazyPassthrough = 0; //dont ask, im lazy
private:
    MobileCharacterSlot aMobileCharacterSlots[2];
    int iLastUp[2] = {-1,-1};
    int iLastDown[2] = {-1,-1};
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