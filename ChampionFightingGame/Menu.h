#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"
#include "Menu.fwd.h"

int menu_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);
int chara_select_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);
int get_sub_selection(int top_selection, int sub_selection);
int load_css(CharaSelectSlot css[32], int *rows, int *cols, SDL_Renderer *renderer);

class MenuItem{
public:
    int destination;
    SDL_Texture* texture;
    SDL_Rect destRect;

    SDL_Texture* texture_description;
    SDL_Rect destRect_description;

    MenuItem();
    MenuItem(string texture_dir, SDL_Renderer *pRenderer, string texture_description_dir  = "resource\\ui\\menu\\main\\missingno.png", int destination = 999);
};

class SubMenuTable {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    int selection;
    int selected_item;
    int item_count;
    Cursor* cursor;

    SDL_Texture* sub_option_text[5];
    SDL_Rect sub_option_rect[5]{ SDL_Rect{0, 0, 0, 0} };

    SubMenuTable();
    SubMenuTable(int selection, SDL_Renderer* pRenderer);
};

class CharaSelectSlot {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    SDL_Rect textRect;
    int chara_kind;
    string chara_name;
    string chara_dir;
    int my_col;
    int my_row;
    bool selectable;

    CharaSelectSlot();
    CharaSelectSlot(SDL_Renderer* pRenderer, int chara_kind, string chara_name, string chara_dir, int my_col, int my_row, bool selectable);
};

class Cursor {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    int pos_y;

    Cursor();
    Cursor(SDL_Renderer* pRenderer);
};

class PlayerCursor {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    PlayerInfo* player_info;
    int my_col;
    int my_row;
    int pos_x;
    int pos_y;

    PlayerCursor();
    PlayerCursor(SDL_Renderer* pRenderer, PlayerInfo* player_info, int init_x, int init_y);
};

class PlayerCSSInfo {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    PlayerInfo* player_info;
    bool selected;
    int selected_slot;

    PlayerCSSInfo();
    PlayerCSSInfo(SDL_Renderer* pRenderer, PlayerInfo* player_info);
};