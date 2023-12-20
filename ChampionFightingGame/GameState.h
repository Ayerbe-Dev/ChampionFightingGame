#pragma once
#include "GameStateConstants.h"
#include <vector>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <any>
#include <stack>
#include "GameTexture.h"
#include "Mouse.h"
#include "SoundPlayer.h"

class MenuObject;
class MenuActivityGroup;

enum RenderType {
    RENDER_TYPE_TEXTURE,
    RENDER_TYPE_CHILD,
    RENDER_TYPE_ACTIVITY
};

struct StateCondition {
    int executed_frame;
    int num_allowed_executions;
};

class GameState{
public:
    GameState();
    ~GameState();
    
    void process_game_state();
    virtual void process_main();

    void render_game_state();
    virtual void render_main();
    void update_state(int game_state = GAME_STATE_MAX, int game_context = GAME_CONTEXT_MAX);
    void push_menu_object(std::string name, int texture_hint = 2, int child_hint = 2, int activity_hint = 2);
    MenuObject& get_menu_object(std::string name);
    void render_menu_object(std::string name);

    void push_menu_pos(glm::vec3 pos);
    void push_menu_orientation(int orientation);
    void push_menu_dimensions(int width, int height);
    void push_menu_dimensions(int texture_id);
    void push_menu_dimensions(std::string texture);

    void push_menu_child(std::string name, int texture_hint = 2, int child_hint = 2, int activity_hint = 2);
    void push_menu_activity_group(std::string name, int* active_index, bool only_render_active, int hint = 2);
    void push_menu_texture(std::string name, std::string path);
    void push_menu_texture(std::string name, Font& font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs);
    void push_menu_texture(std::string name, const GameTexture& that);
    void push_menu_texture(std::string name, unsigned int texture_id);

    void push_menu_int_var(std::string name, int val);
    void push_menu_float_var(std::string name, float val);
    void push_menu_bool_var(std::string name, bool val);
    void push_menu_string_var(std::string name, std::string val);
    void push_menu_ptr_var(std::string name, void* val);

    void push_menu_up_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_down_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_left_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_right_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_select_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_start_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_back_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_any_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_process_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_pre_render_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_post_render_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_on_selected_event_function(std::function<void(MenuObject* menu_object)> function);
    void push_menu_on_deselected_event_function(std::function<void(MenuObject* menu_object)> function);

    void pop_menu_stack();

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

    bool execute_if(std::string name, int num_allowed_executions, bool condition);
    bool execute_wait(std::string name, int num_allowed_executions, unsigned int frames);

    bool looping;
    int game_context;

    std::vector<MenuObject> menu_objects;
    std::unordered_map<std::string, unsigned int> menu_object_map;

    std::stack<MenuObject*> object_stack;
    std::stack<MenuActivityGroup*> activity_group_stack;
    std::stack<bool> last_push_type_stack;

    GameTexture* last_pushed_texture;

    Mouse mouse;

    int player_id;
    int sub_state;
    
    int internal_frame;
    int prev_executed_frame;
    int internal_state;
    int prev_internal_state;
    std::unordered_map<std::string, StateCondition> conditions;
};

class MenuActivityGroup {
public:
    MenuActivityGroup();
    MenuActivityGroup(GameState* owner, MenuObject* parent, std::string name, int* active_index, bool only_render_active, int hint);
    MenuActivityGroup(MenuActivityGroup& other);
    MenuActivityGroup(const MenuActivityGroup& other);
    MenuActivityGroup(MenuActivityGroup&& other) noexcept;
    MenuActivityGroup(const MenuActivityGroup&& other) noexcept;
    ~MenuActivityGroup();

    void process();
    void render();
    void add_child(std::string name, int texture_hint, int child_hint, int activity_hint);
    int num_children();
    MenuObject& get_child(std::string name);
    MenuObject& get_child(int idx);

    MenuObject& get_active_child();
    int get_active_child_index();

    void set_active_child(std::string name);
    void set_active_child(int idx);

    void inc_active_child(int idx);

    int* active_index;
    int prev_active_index;
    bool only_render_active;
    friend class GameState;
    friend class MainMenu;
private:
    GameState* owner;
    MenuObject* parent;
    std::string name;
    std::unordered_map<std::string, unsigned int> child_map;
protected:
    std::vector<MenuObject> children;
};

class MenuObject {
public:
    MenuObject();
    MenuObject(GameState* owner, MenuObject* parent, std::string name, int texture_hint, int child_hint, int activity_hint);
    MenuObject(MenuObject& other);
    MenuObject(const MenuObject& other);
    MenuObject(MenuObject&& other) noexcept;
    MenuObject(const MenuObject&& other) noexcept;
    ~MenuObject();

    void render();
    void add_child(std::string name, int texture_hint, int child_hint, int activity_hint);
    void add_activity_group(std::string name, int* active_index, bool only_render_active, int hint);
    void add_texture(std::string name, std::string path);
    void add_texture(std::string name, Font &font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs);
    void add_texture(std::string name, const GameTexture& that);
    void add_texture(std::string name, unsigned int texture_id);
    int num_children();
    int num_textures();
    MenuObject& get_child(std::string name);
    MenuObject& get_child(int idx);
    MenuActivityGroup& get_activity_group(std::string name);
    MenuActivityGroup& get_activity_group(int idx);
    GameTexture& get_texture(std::string name);
    GameTexture& get_texture(int idx);
    std::string get_name();

    void set_active_sibling(std::string name);
    void set_active_sibling(int idx);

    void set_orientation(int orientation);
    glm::vec3 get_pos();
    void add_pos(glm::vec3 pos);
    void set_pos(glm::vec3 pos);
    void set_pos(glm::vec3 pos, int frames);
    void set_dimensions(int width, int height);
    void set_dimensions(int texture_id);
    void set_dimensions(std::string name);

    void push_int_var(std::string name, int val);
    void push_float_var(std::string name, float val);
    void push_bool_var(std::string name, bool val);
    void push_string_var(std::string name, std::string val);
    void push_ptr_var(std::string name, void* val);
    int& int_var(std::string name);
    float& float_var(std::string name);
    bool& bool_var(std::string name);
    std::string& string_var(std::string name);
    void* ptr_var(std::string name);

    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    void event_select_press();
    void event_start_press();
    void event_back_press();
    void event_any_press();
    void event_process();
    void event_on_selected();
    void event_on_deselected();

    SoundPlayer sound_player;

    friend class GameState;
    friend class MainMenu;
    friend class StageSelect;
    friend class MenuActivityGroup;
private:
    std::string name;
    std::unordered_map<std::string, int> int_vars;
    std::unordered_map<std::string, float> float_vars;
    std::unordered_map<std::string, bool> bool_vars;
    std::unordered_map<std::string, std::string> string_vars;
    std::unordered_map<std::string, void*> ptr_vars;
    TargetVar<glm::vec3> pos;
    glm::vec3 orientated_pos;
    int orientation;
    int width;
    int height;
    std::unordered_map<std::string, unsigned int> child_map;
    std::unordered_map<std::string, unsigned int> texture_map;
    std::unordered_map<std::string, unsigned int> activity_group_map;
    std::vector<std::pair<void*, RenderType>> render_vec;
    std::list<int> active_indices;
protected:
    GameState* owner;
    MenuObject* parent;

    std::vector<MenuObject> children;
    std::vector<GameTexture> textures;
    std::vector<MenuActivityGroup> activity_groups;

    std::function<void(MenuObject* menu_object)> up_event_function;
    std::function<void(MenuObject* menu_object)> down_event_function;
    std::function<void(MenuObject* menu_object)> left_event_function;
    std::function<void(MenuObject* menu_object)> right_event_function;
    std::function<void(MenuObject* menu_object)> select_event_function;
    std::function<void(MenuObject* menu_object)> start_event_function;
    std::function<void(MenuObject* menu_object)> back_event_function;
    std::function<void(MenuObject* menu_object)> any_event_function;

    std::function<void(MenuObject* menu_object)> process_function;
    std::function<void(MenuObject* menu_object)> pre_render_function;
    std::function<void(MenuObject* menu_object)> post_render_function;
    std::function<void(MenuObject* menu_object)> on_selected_event_function;
    std::function<void(MenuObject* menu_object)> on_deselected_event_function;
};