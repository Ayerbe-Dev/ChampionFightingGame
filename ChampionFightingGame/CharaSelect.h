#pragma once
#include <iostream>
#include "Player.h"
#include "GameTexture.h"
#include "GameMenu.h"
#include "GameManager.h"
#include "CharaKind.h"

#define CSS_SLOTS 32
#define NUM_COLS 10
#define NUM_ROWS 4
#define CSS_WIDTH WINDOW_WIDTH * 0.8
#define CSS_HEIGHT WINDOW_HEIGHT / 2.5

void chara_select_main();

class CssSlot {
public:
    GameTexture texture;
    bool is_initialized();
    void init(int id, std::string textureDir, std::string name);
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
    std::string texture_dir = "";
    std::string name = "";
};

class CssCursor{
public:
    void init(std::string texture_dir);
    void set_target(int x, int y);
    void render();
    GameTexture texture;
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

    bool load_css();

    void add_slot(int id, std::string cardDir, std::string cardName);
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

    Player *player[2];
    GameTexture mobile_css_slots[2];
    bool mobile_slots_active[2] = { false };
    CssSlot chara_slots[CSS_SLOTS];
    CssCursor cursors[2];
private:
    GameTexture background_texture;
    GameTexture big_bar_texture;
    GameTexture top_bar_texture;
    CssSlot big_chara_slots[2];
    CssSlot *chara_slots_ordered[10][4];
    bool is_last_input_right[2]{ false };
};