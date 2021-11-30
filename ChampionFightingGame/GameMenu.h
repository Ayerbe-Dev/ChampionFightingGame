#pragma once

class GameMenu{
public:
    virtual void event_up_press();
    virtual void event_down_press();
    virtual void event_left_press();
    virtual void event_right_press();
    virtual void event_select_press();
    virtual void event_start_press();
    virtual void event_back_press();

    bool* looping;
    int* game_state;
    int player_id{0};
};