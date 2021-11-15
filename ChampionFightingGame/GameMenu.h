#pragma once

class GameMenu{
public:
    virtual void GAME_MENU_traverse_up();
    virtual void GAME_MENU_traverse_down();
    virtual void GAME_MENU_traverse_left();
    virtual void GAME_MENU_traverse_right();
    virtual void GAME_MENU_traverse_select();
    virtual void GAME_MENU_traverse_back();
    virtual void GAME_MENU_traverse_start();

    int player_id{0};
};