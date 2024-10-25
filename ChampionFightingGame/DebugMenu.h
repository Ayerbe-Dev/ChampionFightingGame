#pragma once
#include <string>
#include "Player.h"
#include "GameState.h"
#include "GameManager.h"
#include "ScreenTexture.h"
#include "WorldTexture.h"
#include "ScreenText.h"

#define DEBUG_MENU_ITEMS_MAX 50
#define DEBUG_MENU_FONT_SIZE 24

enum{
    DEBUG_LIST_NOT_SELECTABLE,
    DEBUG_LIST_SELECTABLE
};

enum{
    DEBUG_ITEM_NOT_ACTIVE,
    DEBUG_ITEM_ACTIVE
};

class DebugMenu : public GameState {
public:
    DebugMenu();
    ~DebugMenu();

    void process_main();
    void render_main();

    GameTexture tex;
    GameTexture text_field;
    GameTexture text;

    ScreenTexture test_screentexture;
    WorldTexture test_worldtexture;
    ScreenText test_screentext;

    GameObject go1, go2;

    std::vector<std::string> debug_messages;
};

void debug_main();