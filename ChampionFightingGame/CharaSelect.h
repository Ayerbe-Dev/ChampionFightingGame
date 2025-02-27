#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "Param.h"

#include "CharaKind.h"
#include "Player.h"

const int CHARA_SELECT_NUM_COLS = 10;

#define CSS_WIDTH WINDOW_WIDTH * 0.8
#define CSS_HEIGHT WINDOW_HEIGHT / 2.5

const int CHARA_SELECTION_STATE_NAME_ENTRY_HOVER = 0;
const int CHARA_SELECTION_STATE_NAME_ENTRY = 1;
const int CHARA_SELECTION_STATE_CONTROLS_HOVER = 2;
const int CHARA_SELECTION_STATE_CONTROLS = 3;
const int CHARA_SELECTION_STATE_CHARA_HOVER = 4;
const int CHARA_SELECTION_STATE_CHARA = 5;
const int CHARA_SELECTION_STATE_READY = 6;

class Font;

void chara_select_main();

class CSS: public Scene {
public:
    CSS();
    ~CSS();

    bool load_css();
    
    void process_main();
    void render_main();

    GameObject stage_demo;
    GameObject demo_models[2];

    std::vector<AnimationTable> demo_anim_tables;

    std::vector<Light*> lights;

    int loaded_chars;
    int thread_loaded_chars;
private:
    void select_slot();
    void select_costume();
    void select_color();
    void select_preferred_chara_kind(PlayerInfo* player_info);
    void load_chara_model_into_main_thread();
};

void gamestate_charaselect_loading_thread(void* charaselect_arg);