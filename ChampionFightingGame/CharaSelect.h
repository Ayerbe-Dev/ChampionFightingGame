#pragma once
#include <SDL.h>
#include "PlayerInfo.h"
#include "GameTexture.h"
#include "GameMenu.h"
#include "GameManager.h"

void chara_select_main(GameManager *game_manager);

class CssSlot {
public:
    GameTextureNew texture;
    bool is_initialized();
    void init(int id, string textureDir, string name);
    void set_x_pos(int x);
    void set_y_pos(int y);
    int get_texture_width();
    int get_chara_kind();
    void render();
    bool is_below(int y);
    bool is_above(int y);

    int my_col{ -1 };
    int my_row{ -1 };
    int chara_kind = CHARA_KIND_MAX;
    bool initialized = false;
    string texture_dir = "";
    string name = "";
};

class CssCursor{
public:
    void init(string texture_dir);
    void set_target(int x, int y);
    void render();
    GameTextureNew texture;
    int target_x = 0;
    int target_y = 0;
private:
    float partial_x=0;
    float partial_y=0;
};

class CSS: public GameMenu{
public:
    CSS();
    ~CSS();
    int load_css();
    void add_slot(int id, string cardDir, string cardName);
    int get_num_slots();

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

    int player_selected_index[2] = { 0 };
    int my_col[2]{0, 0};
    int my_row[2]{ 0, 0 };

    int num_slots;
    int num_cols;
    int num_rows;
    int cols_offset;

    PlayerInfo *player_info[2];
    GameTextureNew mobile_css_slots[2];
    bool mobile_slots_active[2] = { false };
    CssSlot chara_slots[CSS_SLOTS];
    CssCursor cursors[2];
private:
    GameTextureNew background_texture;
    GameTextureNew big_bar_texture;
    GameTextureNew top_bar_texture;
    CssSlot big_chara_slots[2];
    CssSlot *chara_slots_ordered[10][4];
    bool is_last_input_right[2]{ false };
};