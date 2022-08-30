#pragma once
#include "GameState.h"
#include <vector>
#include <chrono>
#include <functional>
#include "utils.h"
#include "GameTexture.h"

class GameLoader;
class MenuObject;

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
    virtual void event_pause_press();
    virtual void event_any_press();

    virtual void process_background();
    virtual void load_game_menu();

    void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
    void inc_thread();
    void frame_delay();
    void frame_delay_check_fps();
    void frame_delay_check_performance();

    int* game_state;
    int* prev_game_state;
    int* game_context;
    int* prev_game_context;
    bool* looping;
    GameLoader* game_loader;

    std::vector<std::vector<MenuObject>> menu_objects;

    std::chrono::steady_clock::time_point last_second;
    std::chrono::steady_clock::time_point ms;
    std::vector<float> average_ticks;
    std::vector<int> tick_frequency;

    int sub_state = GAME_SUBSTATE_NONE;
    int player_id{0};
    int frame;
    int fps;
    Font fps_font;
    GameTexture fps_counter;
    GameTexture fps_texture;
    int prev_fps;
};

class MenuObject {
public:
    MenuObject();
    MenuObject(GameMenu* owner, MenuObject* parent, bool render_all_children);

    void render();
    void destroy();
    void add_child(bool render_all_children);
    void add_texture(std::string path);
    void add_texture(Font font, std::string text, glm::vec4 rgba, float border_x = 0.0, float border_y = 0.0);
    void add_texture(const GameTexture& that);
    void add_texture(unsigned int texture_id);

    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    void event_select_press();
    void event_start_press();
    void event_back_press();
    void event_any_press();
    void process();

    std::function<void(MenuObject* menu_object)> up_event;
    std::function<void(MenuObject* menu_object)> down_event;
    std::function<void(MenuObject* menu_object)> left_event;
    std::function<void(MenuObject* menu_object)> right_event;
    std::function<void(MenuObject* menu_object)> select_event;
    std::function<void(MenuObject* menu_object)> start_event;
    std::function<void(MenuObject* menu_object)> back_event;
    std::function<void(MenuObject* menu_object)> any_event;

    std::function<void(MenuObject* menu_object)> process_event;

    GameMenu* owner;
    MenuObject* parent;

    std::vector<MenuObject> children;
    std::vector<GameTexture> textures;

    GameTexture cursor; //I can't think of a single reason for a menu object
    //to actually use its cursor. However, I CAN think of reasons for it to
    //use its PARENT'S cursor, so here we are.

    int active_child;

    bool render_all_children;
};