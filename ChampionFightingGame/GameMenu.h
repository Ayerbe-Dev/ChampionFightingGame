#pragma once
#include "utils.h"

class GameMenu{
public:
    GameMenu();
    virtual void event_up_press();
    virtual void event_down_press();
    virtual void event_left_press();
    virtual void event_right_press();
    virtual void event_select_press();
    virtual void event_start_press();
    virtual void event_back_press();
    virtual void event_any_press();

    virtual void process_background(SDL_Texture* background);

    bool* looping;
    int* game_state;
    int sub_state;
    int player_id{0};
    SDL_Texture* background_layer[MAX_LAYERS];
};