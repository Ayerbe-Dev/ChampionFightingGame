#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "PlayerInfo.h"

#define DEBUG_MENU_ITEMS_MAX 20
#define DEBUG_MENU_FONT_SIZE 24

enum{
    DEBUG_LIST_NOT_SELECTABLE,
    DEBUG_LIST_SELECTABLE
};

enum{
    DEBUG_ITEM_NOT_ACTIVE,
    DEBUG_ITEM_ACTIVE
};

class DebugItem{
public:
    int state = DEBUG_ITEM_NOT_ACTIVE;
    SDL_Texture *pTexture;
    SDL_Rect destRect;

    SDL_Texture *pTextureSelect;
    SDL_Rect destRectSelect;

    SDL_Renderer *pRenderer;
    TTF_Font *pFont;

    DebugItem();
    void preLoad(SDL_Renderer *pRenderer, TTF_Font *pFont);
    void generateTexture(string message = "no message");
};
class DebugList{
public:
    int selection = 0;
    DebugItem debugItems[DEBUG_MENU_ITEMS_MAX];
    SDL_Renderer *pRenderer;
    TTF_Font *pFont;
    DebugList();
    DebugList(SDL_Renderer *pRenderer, TTF_Font *pFont);
    void addEntry(string message, int selectable = DEBUG_LIST_SELECTABLE);
    void render();
};

TTF_Font *loadDebugFont(string fontname = "FiraCode-Regular.ttf");
int debugMenu(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2], int gamestate);

