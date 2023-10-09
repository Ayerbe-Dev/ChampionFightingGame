#pragma once
#include "GameStateConstants.h"
#include <vector>
#include <chrono>
#include <functional>
#include "GameTexture.h"
#include "Mouse.h"

class GameLoader;
class MenuObject;

class GameState{
public:
    GameState();
    ~GameState();
    
    void process_game_state();
    virtual void process_main();

    void render_game_state();
    virtual void render_main();


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

    void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
    void inc_thread();

    bool looping;
    int game_context;
    GameLoader* game_loader;

    std::vector<std::vector<MenuObject>> menu_objects;

    Mouse mouse;

    int sub_state = GAME_STATE_NONE;
    int player_id;
};

class MenuObject {
public:
    MenuObject();
    MenuObject(GameState* owner, MenuObject* parent, bool render_all_children);
    MenuObject(MenuObject& other);
    MenuObject(MenuObject&& other) noexcept;
    ~MenuObject();

    void render();
    void add_child(bool render_all_children);
    void add_texture(std::string path);
    void add_texture(Font &font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs);
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

    GameState* owner;
    MenuObject* parent;

    std::vector<MenuObject> children;
    std::vector<GameTexture> textures;

    GameTexture cursor; //I can't think of a single reason for a menu object
    //to actually use its cursor. However, I CAN think of reasons for it to
    //use its PARENT'S cursor, so here we are.

    int active_child;

    bool render_all_children;
};