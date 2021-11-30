#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "PlayerInfo.h"
#include "GameMenu.h"
#include "GameManager.h"

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
class debug_list: public GameMenu{
public:
    int selection = 0;
    int x_offset = 15;
    DebugItem debugItems[DEBUG_MENU_ITEMS_MAX];
    TTF_Font *pFont;
    debug_list();
    debug_list(TTF_Font *pFont, int x_offset = 15);
    void init(TTF_Font *pFont, int x_offset = 15);
    void addEntry(string message, int selectable = DEBUG_LIST_SELECTABLE, int destination = 888);
    void render();
    void event_down_press();
    void event_up_press();
    void event_start_press();
    int getDestination();
};

TTF_Font *loadDebugFont(string fontname = "FiraCode-Regular.ttf");
void debugMenu(GameManager *game_manager);