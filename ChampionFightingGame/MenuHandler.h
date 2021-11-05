#pragma once
#include "DebugMenu.h"
#include "PlayerInfo.h"

typedef void (DebugList::*DBMtraversalFunction)();

class MenuHandler{
public:
    MenuHandler();
    MenuHandler(DebugList *pHandlerTarget, PlayerInfo *pPlayerInfo1, PlayerInfo *pPlayerInfo2);

    void setEventMenuUp(DBMtraversalFunction traversal);
    void setEventMenuDown(DBMtraversalFunction traversal);
    void setEventMenuLeft(DBMtraversalFunction traversal);
    void setEventMenuRight(DBMtraversalFunction traversal);
    /*
        Each menu has a finisher function which will be responsible for deciding if its ok for the menu to exit.
        Then it will run the last scripts it needs to exit properly
    */
    void setEventMenuFinish(DBMtraversalFunction traversal);
    
    //requires SDL_PumpEvents(); to be called before this function.
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
    DBMtraversalFunction nsmfUpTraversal;
    DBMtraversalFunction nsmfDownTraversal;
    DBMtraversalFunction nsmfLeftTraversal;
    DBMtraversalFunction nsmfRightTraversal;
    DBMtraversalFunction nsmfFinisher;

    bool bUpDefined=false, bDownDefined=false, bLeftDefined=false, bRightDefined=false, bFinisherDefined=false;

    DebugList *pHandlerTarget;

    unsigned int iHoldFrames=0, iInitialDelay = 70, iRepeatDelay = 20;

    bool canRepeatKeys();
};