#pragma once
#include "utils.h"
#include <vector>

class GameLoader;

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

    virtual void process_background();
    void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
    void inc_thread();
    void frame_delay();
    void frame_delay_check_performance();

    int* game_state;
    int* prev_game_state;
    int* game_context;
    int* prev_game_context;
    bool* looping;
    GameLoader* game_loader;
    int ticks = 0;
    vector<int> average_ticks;
    vector<int> tick_frequency;

    int sub_state = GAME_SUBSTATE_NONE;
    int player_id{0};
};