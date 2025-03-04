#pragma once
#include "Scene.h"
#include <string>
#include "Player.h"
#include "SceneElement.h"

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

class DebugMenu : public Scene {
public:
    DebugMenu();
    ~DebugMenu();

    void process_main();
    void render_main();

    GameObject go1, go2;

    std::vector<std::string> debug_messages;
};

void debug_main();