#pragma once
#include <SDL/SDL.h>
#include <string>
#include "Player.h"
#include "GameMenu.h"
#include "GameManager.h"

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

class DebugMenu : public GameMenu {
public:
    DebugMenu();
    ~DebugMenu();

    std::vector<std::string> debug_messages;
};

void debug_main();