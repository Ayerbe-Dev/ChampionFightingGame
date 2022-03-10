#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "PlayerInfo.h"
#include "GameMenu.h"
#include "GameManager.h"

void menu_main(GameManager *game_manager);
int get_sub_selection(int top_selection, int sub_selection);

class MenuItem{
public:
    int destination;
    float rot = 0;

    GameTexture image_texture;
    GameTexture name_texture;

    MenuItem();
    void init(std::string texture_dir, std::string texture_description_dir  = "resource/ui/menu/main/missingno.png", int destination = 999);
    void destroy();
};

class SubMenuTable {
public:
    int selection;
    int selected_item;
    int item_count;

    GameTexture table;
    GameTexture sub_text[5];
    GameTexture cursor;

    SubMenuTable();
    SubMenuTable(int selection);
    void destroy();
};

class MainMenu: public GameMenu{
public:
    MainMenu();
    ~MainMenu();

    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    void event_select_press();
    void event_back_press();
    void event_start_press();

    void process_background();

    void render();
    void process_submenu_tables();
    SubMenuTable* sub_menu_tables[5];
private:
    float theta = 0;
    float offset = 3.14 / 10;
    float magnitude = WINDOW_WIDTH / 1.25;  //this is about 45 degrees
    int top_selection = -2; //first option, dont ask; 5 opts --> -2 -1 0 1 2 represen them
    int sub_selection = GAME_STATE_BATTLE;
    int menu_level = MENU_LEVEL_TOP;
    int sub_type = SUB_MENU_VS;
    MenuItem menu_items[5];
    GameTexture background_texture;
};
