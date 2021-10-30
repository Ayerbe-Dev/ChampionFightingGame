#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"

int menu_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);
int chara_select_main(SDL_Renderer* pRenderer, SDL_Window *window, PlayerInfo player_info[2]);
int get_sub_selection(int top_selection, int sub_selection);

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

class Cursor {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    int pos_y;

    Cursor();
    Cursor(SDL_Renderer* pRenderer);
};

class SubMenuTable {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    int selection;
    int selected_item;
    int item_count;
    Cursor *cursor;

    SubMenuTable();
    SubMenuTable(int selection, SDL_Renderer* pRenderer);
};