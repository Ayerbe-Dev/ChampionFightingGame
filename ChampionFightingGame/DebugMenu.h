#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "PlayerInfo.h"
#include "GameMenu.h"

#define DEBUG_MENU_ITEMS_MAX 50
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
    int selectable = DEBUG_LIST_SELECTABLE;
    int destination = 999;
    SDL_Texture *pTexture;
    SDL_Rect destRect;

    SDL_Texture *pTextureSelect;
    SDL_Rect destRectSelect;

    TTF_Font *pFont;

    DebugItem();
    //~DebugItem();
    void preLoad(TTF_Font *pFont);
    void generateTexture(string message = "no message");
};
class DebugList: public GameMenu{
public:
    int selection = 0;
    int x_offset = 15;
    DebugItem debugItems[DEBUG_MENU_ITEMS_MAX];
    TTF_Font *pFont;
    DebugList();
    DebugList(TTF_Font *pFont, int x_offset = 15);
    void init(TTF_Font *pFont, int x_offset = 15);
    //~DebugList();
    void addEntry(string message, int selectable = DEBUG_LIST_SELECTABLE, int destination = 888);
    void render();
    void GAME_MENU_traverse_down();
    void GAME_MENU_traverse_up();
    void GAME_MENU_traverse_start();
    int getDestination();
    bool debugging = true;
};

TTF_Font *loadDebugFont(string fontname = "FiraCode-Regular.ttf");
int debugMenu(PlayerInfo player_info[2], int gamestate);