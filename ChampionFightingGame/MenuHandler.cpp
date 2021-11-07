#include "MenuHandler.h"
#include "DebugMenu.h"

MenuHandler::MenuHandler(){};
MenuHandler::MenuHandler(DebugList *pHandlerTarget, PlayerInfo *pPlayerInfo1, PlayerInfo *pPlayerInfo2){
    this->pHandlerTarget = pHandlerTarget;
    this->pPlayerInfoArray[0] = pPlayerInfo1;
    this->pPlayerInfoArray[1] = pPlayerInfo1;
};

void MenuHandler::setEventMenuUp(DBMtraversalFunction traversal){this->nsmfUpTraversal = traversal; bUpDefined=true;};
void MenuHandler::setEventMenuDown(DBMtraversalFunction traversal){this->nsmfDownTraversal = traversal; bDownDefined=true;};
void MenuHandler::setEventMenuLeft(DBMtraversalFunction traversal){this->nsmfLeftTraversal = traversal; bLeftDefined=true;};
void MenuHandler::setEventMenuRight(DBMtraversalFunction traversal){this->nsmfRightTraversal = traversal; bRightDefined=true;};
void MenuHandler::setEventMenuFinish(DBMtraversalFunction traversal){this->nsmfFinisher = traversal; bFinisherDefined=true;};

void MenuHandler::handleMenu(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        //On Press
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_UP) && bUpDefined){
            (pHandlerTarget->*(nsmfUpTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_DOWN) && bDownDefined){
            (pHandlerTarget->*(nsmfDownTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_LEFT) && bLeftDefined){
            (pHandlerTarget->*(nsmfLeftTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_RIGHT) && bRightDefined){
            (pHandlerTarget->*(nsmfRightTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_START) || pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_SELECT)){
            (pHandlerTarget->*(nsmfFinisher))();
        }

        //On hold
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_UP) || pPlayerInfoArray[i]->check_button_on(BUTTON_DOWN) || pPlayerInfoArray[i]->check_button_on(BUTTON_LEFT) || pPlayerInfoArray[i]->check_button_on(BUTTON_RIGHT)){
            iHoldFrames++;
        } else {
            iHoldFrames=0;
        }

        if (pPlayerInfoArray[i]->check_button_on(BUTTON_UP) && bUpDefined && canRepeatKeys()){
            (pHandlerTarget->*(nsmfUpTraversal))();
            iHoldFrames = iInitialDelay - iRepeatDelay;
        }
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_DOWN) && bDownDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfDownTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_LEFT) && bLeftDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfLeftTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_RIGHT) && bRightDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfRightTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        }

	}
};

bool MenuHandler::canRepeatKeys(){
    return (iHoldFrames > iInitialDelay);
}

void MenuHandler::setRepeatDelay(unsigned int delay){
    iRepeatDelay = delay;
}

void MenuHandler::setInitialDelay(unsigned int delay){
    iInitialDelay = delay;
}