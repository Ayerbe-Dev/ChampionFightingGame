#pragma once
#include "DebugMenu.h"
#include "PlayerInfo.h"
#include "CharaSelect.h"
#include "TitleScreen.h"
#include "GameMenu.h"

typedef void (DebugList::*DBMtraversalFunction)();
typedef void (CSS::*CSStraversalFunction)();
typedef void (TitleScreen::*TitleTraversalFunction)();

typedef void (GameMenu::*MenuTraversalFunction)();

class MenuHandler{
public:
    MenuHandler(GameMenu *menu_target, PlayerInfo player_info[2]);
    /*
        requires SDL_PumpEvents(); to be called before this function.
    */
    void handleMenu();

    /*
        Sets the number of frames before repeat mode starts
        \param delay Number of frames
    */
    void setInitialDelay(unsigned int delay);

    /*
        Sets the number of frames between each key press
        \param delay Number of frames
    */
    void setRepeatDelay(unsigned int delay);
private:
    PlayerInfo *pPlayerInfoArray[2];
    GameMenu *menu_target;

    int iHoldFrames[2]{0,0}, iInitialDelay{70}, iRepeatDelay{20};

    bool canRepeatKeys();
};