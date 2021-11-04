#include "MenuHandler.h"
#include "DebugMenu.h"

MenuHandler::MenuHandler(){};
MenuHandler::MenuHandler(DebugList *pHandlerTarget, PlayerInfo *pPlayerInfo1, PlayerInfo *pPlayerInfo2){
    this->pHandlerTarget = pHandlerTarget;
    this->pPlayerInfoArray[0] = pPlayerInfo1;
    this->pPlayerInfoArray[1] = pPlayerInfo1;
};

void MenuHandler::mapUp(DBMtraversalFunction traversal){this->nsmfUpTraversal = traversal; bUpDefined=true;};
void MenuHandler::mapDown(DBMtraversalFunction traversal){this->nsmfDownTraversal = traversal; bDownDefined=true;};
void MenuHandler::mapLeft(DBMtraversalFunction traversal){this->nsmfLeftTraversal = traversal; bLeftDefined=true;};
void MenuHandler::mapRight(DBMtraversalFunction traversal){this->nsmfRightTraversal = traversal; bRightDefined=true;};
void MenuHandler::mapFinisher(DBMtraversalFunction traversal){this->nsmfFinisher = traversal; bFinisherDefined=true;};

void MenuHandler::handleMenu(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_UP) && bUpDefined){
            (pHandlerTarget->*(nsmfUpTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_DOWN) && bDownDefined){
            (pHandlerTarget->*(nsmfDownTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_LEFT) && bLeftDefined){
            (pHandlerTarget->*(nsmfLeftTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_RIGHT) && bRightDefined){
            (pHandlerTarget->*(nsmfRightTraversal))();
        } else if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_LP) && bUpDefined){
            (pHandlerTarget->*(nsmfFinisher))();
        }
	}
};