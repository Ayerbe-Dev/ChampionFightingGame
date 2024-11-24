#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "GameState.h"
#include "GameManager.h"
#include "GameTexture.h"
#include "MainMenuConstants.h"

void main_menu_main();

class MainMenu: public GameState{
public:
    MainMenu();
    ~MainMenu();

    void process_main();
    void render_main();

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

    int top_selection = SUB_MENU_VS;
    int sub_selection = GAME_STATE_BATTLE;
    int menu_level = MENU_LEVEL_TOP;
};
