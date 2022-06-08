#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "GameMenu.h"
#include "GameManager.h"
#include "GameTexture.h"

void menu_main();
int get_sub_selection(int top_selection, int sub_selection);

class MenuItem {
public:
    MenuItem();
    void init(Font font, std::string text, int destination, std::string texture_dir = "resource/ui/menu/main/missingno.png");
    void destroy();

    int destination;

    GameTexture texture;
    GameTexture sub_menu_name;
};

class SubMenuTable {
public:
    int selection;
    int selected_item;

    std::vector<GameTexture> sub_text;
    GameTexture cursor;

    SubMenuTable();
    SubMenuTable(int selection);
    void destroy();
};

class MainMenu: public GameMenu{
public:
    MainMenu();
    ~MainMenu();

    void load_game_menu();

    void process_main();
    void render();
    void process_background();

    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    void event_select_press();
    void event_back_press();
    void event_start_press();
private:
    //Menu Wheel Variables
    float theta;
    float offset;
    float magnitude;
    float turn_frames;

    int menu_frame;

    int top_selection = -2; //first option, dont ask; 5 opts --> -2 -1 0 1 2 represen them
    int sub_selection = GAME_STATE_BATTLE;
    int menu_level = MENU_LEVEL_TOP;
    int sub_type = SUB_MENU_VS;

    GameTexture table;
    GameTexture background_texture;

    MenuItem menu_items[5];
    SubMenuTable* sub_menu_tables[5];
};
