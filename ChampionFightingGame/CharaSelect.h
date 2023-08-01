#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Camera.h"
#include "CharaSelectConstants.h"
#include "Light.h"
#include "Param.h"

#include "CharaKind.h"
#include "Player.h"

class Font;

void chara_select_main();

class CssSlot {
public:
    CssSlot();
    CssSlot(ParamTable param_table);

    void init(ParamTable param_table);
    void render();

    int get_chara_kind() const;

    int my_col;
    int my_row;
    int chara_kind;
    std::string resource_dir;
    std::string chara_name;

    std::vector<std::pair<std::string, int>> costumes;

    GameTexture render_texture;
    AnimationTable anim_table;
};

class CssPlayer {
public:
    CssPlayer();
    void init(PlayerInfo* player_info);

    int my_row;
    int my_col;

    int selected_index;
    int selected_costume;
    int selected_color;

    CharaSelectionState state;

    GameTexture mobile_css_slot;
    GameObject demo_model;
};

class CSS: public GameState{
public:
    CSS();
    ~CSS();

    bool load_css();

    void event_select_press();
    void event_back_press();
    void event_start_press();
    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    
    void process_main();
    void render_main();

    int get_chara_kind(int player_id);

    int num_cols;
    int num_rows;
    int cols_offset;

    Player *player[2];
    CssPlayer css_player[2];

    GameObject stage_demo;

    std::vector<CssSlot> chara_slots;
    std::vector<Light> lights;

    int loaded_chars;
    int thread_loaded_chars;
private:
    void select_slot();
    void select_costume();
    void select_color();

    void select_default_chara_kind(int chara_kind);

    GameTexture big_chara_slots[2];
    CssSlot *chara_slots_ordered[10][4];
    bool is_last_input_right[2]{ false };
    
    int gbuffer_window_counter;
    glm::vec4 gbuffer_mul;
    glm::vec4 gbuffer_mul_offset;
};

void gamestate_charaselect_loading_thread(void* charaselect_arg);