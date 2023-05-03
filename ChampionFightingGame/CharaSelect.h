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
    CssSlot(int id, std::string resource_name, std::string chara_name);
    void init(int id, std::string resource_name, std::string chara_name);
    void set_x_pos(int x);
    void set_y_pos(int y);
    int get_texture_width();
    int get_chara_kind();
    void render();
    bool is_below(int y);
    bool is_above(int y);

    GameTexture texture;
    int my_col;
    int my_row;
    int chara_kind;
    std::string resource_dir;
    std::string chara_name;
};;

class CSS: public GameState{
public:
    CSS();
    ~CSS();

    bool load_css();
    void add_chara_slot(ParamTable param_table);

    void event_select_press();
    void event_back_press();
    void event_start_press();
    void event_up_press();
    void event_down_press();
    void event_left_press();
    void event_right_press();
    
    void render();

    int get_chara_kind(int player_id);
    void select_slot();
    void find_prev_chara_kind(int chara_kind);

    int player_selected_index[2];
    int my_col[2];
    int my_row[2];

    int num_cols;
    int num_rows;
    int cols_offset;

    Player *player[2];
    GameTexture mobile_css_slots[2];
    bool mobile_slots_active[2] = { false };
    std::vector<CssSlot> chara_slots;
private:
    CssSlot big_chara_slots[2];
    CssSlot *chara_slots_ordered[10][4];
    bool is_last_input_right[2]{ false };
};