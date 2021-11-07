#pragma once
#include "DebugMenu.h"
#include "PlayerInfo.h"
#include "CSSMenu.h"

typedef void (DebugList::*DBMtraversalFunction)();
typedef void (CSSMenu::*CSStraversalFunction)();
class MenuHandler{
public:
    MenuHandler();
    MenuHandler(DebugList *pHandlerTarget, PlayerInfo *pPlayerInfo1, PlayerInfo *pPlayerInfo2);
    MenuHandler(CSSMenu *pHandlerTarget, PlayerInfo aPlayerInfo[2]);

    void setEventMenuUp(DBMtraversalFunction traversal);
    void setEventMenuDown(DBMtraversalFunction traversal);
    void setEventMenuLeft(DBMtraversalFunction traversal);
    void setEventMenuRight(DBMtraversalFunction traversal);
    /*
        Each menu has a finisher function which will be responsible for deciding if its ok for the menu to exit.
        Then it will run the last scripts it needs to exit properly
    */
    void setEventMenuFinish(DBMtraversalFunction traversal);
    
    void setEventMenuUp(CSStraversalFunction traversal);
    void setEventMenuDown(CSStraversalFunction traversal);
    void setEventMenuLeft(CSStraversalFunction traversal);
    void setEventMenuRight(CSStraversalFunction traversal);
    void setEventMenuFinish(CSStraversalFunction traversal);
    void setEventMenuBack(CSStraversalFunction traversal);
    void setEventMenuSelect(CSStraversalFunction traversal);

    //requires SDL_PumpEvents(); to be called before this function.
    void handleMenu();
    void handleCSSMenu();

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
    DBMtraversalFunction nsmfUpTraversal;
    DBMtraversalFunction nsmfDownTraversal;
    DBMtraversalFunction nsmfLeftTraversal;
    DBMtraversalFunction nsmfRightTraversal;
    DBMtraversalFunction nsmfFinisher;

    CSStraversalFunction nsmfCssUpTraversal;
    CSStraversalFunction nsmfCssDownTraversal;
    CSStraversalFunction nsmfCssLeftTraversal;
    CSStraversalFunction nsmfCssRightTraversal;
    CSStraversalFunction nsmfCssFinisher;
    CSStraversalFunction nsmfCssSelect;
    CSStraversalFunction nsmfCssBack;

    bool bUpDefined=false, bDownDefined=false, bLeftDefined=false, bRightDefined=false, bFinisherDefined=false, bBackDefined=false, bSelectDefined=false;

    CSSMenu * pCssHandlerTarget;
    DebugList *pHandlerTarget;

    unsigned int iHoldFrames=0, iInitialDelay = 70, iRepeatDelay = 20;

    bool canRepeatKeys();
};