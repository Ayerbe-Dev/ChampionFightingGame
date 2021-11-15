#include "MenuHandler.h"
#include "DebugMenu.h"
#include "CharaSelect.h"

MenuHandler::MenuHandler(GameMenu *menu_target, PlayerInfo player_info[2]){
    this->menu_target = menu_target;
    this->pPlayerInfoArray[0] = &player_info[0];
    this->pPlayerInfoArray[1] = &player_info[1];
}



void MenuHandler::handleMenu(){
    for (int i = 0; i < 2; i++) {
        pPlayerInfoArray[i]->update_controller();
        pPlayerInfoArray[i]->update_buttons(SDL_GetKeyboardState(nullptr));
        
        //On Press
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_UP)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_up))();
        } 
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_DOWN)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_down))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_LEFT)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_left))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_RIGHT)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_right))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_START)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_start))();
        }
        if (pPlayerInfoArray[i]->check_button_trigger(BUTTON_MENU_SELECT)){
            menu_target->id_player_last_input = i;
            (menu_target->*(&GameMenu::GAME_MENU_traverse_select))();
        }

        //On hold
        if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT) || pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT)){
            iHoldFrames[i]++;
        } 
        else {
            iHoldFrames[i] = 0;
        }

        if (canRepeatKeys()){
            if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_UP)){
                (menu_target->*(&GameMenu::GAME_MENU_traverse_up))();
                iHoldFrames[i] = iInitialDelay - iRepeatDelay;
            }
            if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_DOWN)){
                (menu_target->*(&GameMenu::GAME_MENU_traverse_down))();
                iHoldFrames[i] = iInitialDelay - iRepeatDelay;
            } 
            if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_LEFT)){
                (menu_target->*(&GameMenu::GAME_MENU_traverse_left))();
                iHoldFrames[i] = iInitialDelay - iRepeatDelay;
            } 
            if (pPlayerInfoArray[i]->check_button_on(BUTTON_MENU_RIGHT)){
                (menu_target->*(&GameMenu::GAME_MENU_traverse_right))();
                iHoldFrames[i] = iInitialDelay - iRepeatDelay;
            }
        }

	}
};

bool MenuHandler::canRepeatKeys(){
    return (iHoldFrames[menu_target->id_player_last_input] > iInitialDelay);
}

void MenuHandler::setRepeatDelay(unsigned int delay){
    iRepeatDelay = delay;
}

void MenuHandler::setInitialDelay(unsigned int delay){
    iInitialDelay = delay;
}