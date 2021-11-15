#include "MenuHandler.h"
#include "DebugMenu.h"
#include "CharaSelect.h"

MenuHandler::MenuHandler(){};
MenuHandler::MenuHandler(DebugList *pHandlerTarget, PlayerInfo *pPlayerInfo1, PlayerInfo *pPlayerInfo2){
    this->pHandlerTarget = pHandlerTarget;
    this->pPlayerInfoArray[0] = pPlayerInfo1;
    this->pPlayerInfoArray[1] = pPlayerInfo1;
};

MenuHandler::MenuHandler(CSS *pHandlerTarget, PlayerInfo aPlayerInfo[2]){
    this->pCssHandlerTarget = pHandlerTarget;
    this->pPlayerInfoArray[0] = &aPlayerInfo[0];
    this->pPlayerInfoArray[1] = &aPlayerInfo[1];
};

MenuHandler::MenuHandler(TitleScreen *p_title_target, PlayerInfo a_player_info[2]){
    this->p_title_target = p_title_target;
    this->pPlayerInfoArray[0] = &a_player_info[0];
    this->pPlayerInfoArray[1] = &a_player_info[1];
}

void MenuHandler::setEventMenuUp(DBMtraversalFunction traversal){this->nsmfUpTraversal = traversal; bUpDefined=true;};
void MenuHandler::setEventMenuDown(DBMtraversalFunction traversal){this->nsmfDownTraversal = traversal; bDownDefined=true;};
void MenuHandler::setEventMenuLeft(DBMtraversalFunction traversal){this->nsmfLeftTraversal = traversal; bLeftDefined=true;};
void MenuHandler::setEventMenuRight(DBMtraversalFunction traversal){this->nsmfRightTraversal = traversal; bRightDefined=true;};
void MenuHandler::setEventMenuFinish(DBMtraversalFunction traversal){this->nsmfFinisher = traversal; bFinisherDefined=true;};

void MenuHandler::setEventMenuUp(CSStraversalFunction traversal){this->nsmfCssUpTraversal = traversal; bUpDefined=true;};
void MenuHandler::setEventMenuDown(CSStraversalFunction traversal){this->nsmfCssDownTraversal = traversal; bDownDefined=true;};
void MenuHandler::setEventMenuLeft(CSStraversalFunction traversal){this->nsmfCssLeftTraversal = traversal; bLeftDefined=true;};
void MenuHandler::setEventMenuRight(CSStraversalFunction traversal){this->nsmfCssRightTraversal = traversal; bRightDefined=true;};
void MenuHandler::setEventMenuFinish(CSStraversalFunction traversal){this->nsmfCssFinisher = traversal; bFinisherDefined=true;};
void MenuHandler::setEventMenuBack(CSStraversalFunction traversal){this->nsmfCssBack = traversal; bBackDefined=true;};
void MenuHandler::setEventMenuSelect(CSStraversalFunction traversal){this->nsmfCssSelect = traversal; bSelectDefined=true;};

void MenuHandler::setEventMenuFinish(TitleTraversalFunction traversal){this->nsmfTitleFinisher = traversal; bFinisherDefined=true;};

void MenuHandler::handleMenu(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        //On Press
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_UP) && bUpDefined){
            (pHandlerTarget->*(nsmfUpTraversal))();
        } 
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_DOWN) && bDownDefined){
            (pHandlerTarget->*(nsmfDownTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_LEFT) && bLeftDefined){
            (pHandlerTarget->*(nsmfLeftTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_RIGHT) && bRightDefined){
            (pHandlerTarget->*(nsmfRightTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_START) || pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_SELECT)){
            (pHandlerTarget->*(nsmfFinisher))();
        }

        //On hold
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT)){
            iHoldFrames++;
        } 
        else {
            iHoldFrames=0;
        }

        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP) && bUpDefined && canRepeatKeys()){
            (pHandlerTarget->*(nsmfUpTraversal))();
            iHoldFrames = iInitialDelay - iRepeatDelay;
        }
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN) && bDownDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfDownTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT) && bLeftDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfLeftTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT) && bRightDefined && canRepeatKeys()){
             (pHandlerTarget->*(nsmfRightTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        }

	}
};

void MenuHandler::handleCSS(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        //On Press
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_UP) && bUpDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssUpTraversal))();
        } 
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_DOWN) && bDownDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssDownTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_LEFT) && bLeftDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssLeftTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_RIGHT) && bRightDefined){
            pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssRightTraversal))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_START) && bFinisherDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssFinisher))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_BACK)  && bBackDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssBack))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_SELECT)  && bSelectDefined){
             pCssHandlerTarget->playerID = i;
            (pCssHandlerTarget->*(nsmfCssSelect))();
        }

        //On hold
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT)){
            iHoldFrames++;
        } else {
            iHoldFrames=0;
        }

        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP) && bUpDefined && canRepeatKeys()){
            (pCssHandlerTarget->*(nsmfCssUpTraversal))();
            iHoldFrames = iInitialDelay - iRepeatDelay;
        }
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN) && bDownDefined && canRepeatKeys()){
             (pCssHandlerTarget->*(nsmfCssDownTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT) && bLeftDefined && canRepeatKeys()){
             (pCssHandlerTarget->*(nsmfCssLeftTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        } 
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT) && bRightDefined && canRepeatKeys()){
             (pCssHandlerTarget->*(nsmfCssRightTraversal))();
             iHoldFrames = iInitialDelay - iRepeatDelay;
        }

	}
};

void MenuHandler::handleTitle(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        //On Press
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_START) && bFinisherDefined){
            (p_title_target->*(nsmfTitleFinisher))();
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