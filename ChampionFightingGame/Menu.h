#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"
#include "Menu.fwd.h"
#include "GameMenu.h"
#include "GameManager.h"

void menu_main(GameManager *game_manager);
int get_sub_selection(int top_selection, int sub_selection);

class MenuItem{
public:
    int destination;
    SDL_Texture* texture;
    SDL_Rect destRect;

    SDL_Texture* texture_description;
    SDL_Rect destRect_description;

    MenuItem();
    MenuItem(string texture_dir, string texture_description_dir  = "resource\\ui\\menu\\main\\missingno.png", int destination = 999);
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
    SubMenuTable(int selection);
};

class Cursor {
public:
    SDL_Texture* texture;
    SDL_Rect destRect;
    int pos_y;

    Cursor();
};

class MainMenu: public GameMenu{
public:
    MainMenu();

    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    void event_select_press();
    void event_back_press();
    void event_start_press();

    void process_background(SDL_Texture *background);

    void init();
    void render();
    void process_submenu_tables();
    SubMenuTable* sub_menu_tables[5];
private:
    float theta = 0;
    float offset = 3.14 / 13;
    float magnitude = WINDOW_WIDTH / 2;  //this is about 45 degrees
    int top_selection = -2; //first option, dont ask; 5 opts --> -2 -1 0 1 2 represen them
    int sub_selection = GAME_STATE_BATTLE;
    int menu_level = MENU_LEVEL_TOP;
    int sub_type = SUB_MENU_VS;
    MenuItem menu_items[5];
    GameTexture background_texture;
};
