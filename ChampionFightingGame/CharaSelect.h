#pragma once
#include <SDL.h>
#include "PlayerInfo.h"
#include "GameTexture.h"

int chara_select_main(PlayerInfo aPlayerInfo[2]);

class FixedCharacterSlot {
public:
    GameTexture gameTexture;
    bool isInitialized();
    void init(int id, string textureDir);
    void setXPosition(int iX);
    void setYPosition(int iY);
    int getTextureWidth();
    int getCharacterId();
    void render();

    bool isBelow(int y);
    bool isAbove(int y);
    int myCol{ -1 };
    int myRow{ -1 };
    int iCharacterId = CHARA_KIND_MAX;
    bool bInitialized = false;
};

class MobileCharacterSlot {
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
    int iAnimationSpeed = 16;
    int targetX = 0, targetY = 0;
    float fPosX = 0, fPosY = 0;
    float fTheta = 0;
    int iAnimTime = 0;
    float fScale = .2;

    float fHorizontalscaleOffset = 0, fVerticalscale = 0;
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

class CSS {
public:
    CSS();
    int loadCSS();
    void addFixedCharacter(int id, string cardDir);
    int getSlotsLength();
    void select();
    void back();
    void start();
    void traverseUp();
    void traverseDown();
    void traverseLeft();
    void traverseRight();
    void render();
    void queryFixedCssSlotPosition(int indexLocation, int* xptr, int* yptr);
    int getExitCode();
    int getCharacterKind(int player);
    void centerSlots();
    void selectIndex();
    void findPrevChara(int chara_kind);

    int aPlayerSelectionIndex[2] = { 0,0 };
    int myCol[2]{0, 0};
    int myRow[2]{ 0, 0 };

    int numCols;
    int numRows;
    int colsOffset;

    int playerID = 0;
    bool bSelecting = true;
    PlayerInfo *aPlayerInfos[2];
private:

    int iExitCode = GAME_STATE_DEBUG_MENU;
    MobileCharacterSlot aMobileCharacterSlots[2];
    GameTexture backgroundTexture;
    GameTexture bigBarTexture;
    GameTexture topBarTexture;
    FixedCharacterSlot aFixedCharacterSlots[CSS_SLOTS];
    FixedCharacterSlot charaSlotsOrdered[10][4];
    bool isLastRight[2]{ false };
};