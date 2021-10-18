#include <iostream>
#include <SDL.h>
#include <stdbool.h>
#include "DebugMenu.h"
int debugMenu()
{

    const unsigned int *keyboard_state;
    const unsigned int *keyboard_state_old;
    bool debugging = true;
    while (debugging)
    {
        keyboard_state = SDL_GetKeyboardState(NULL);
        if (wasPressed(SDL_SCANCODE_DOWN, keyboard_state, keyboard_state_old))
        {
            //std::cout << "Down Pressed" << std::endl;
        }
    }
}

bool wasPressed(int keynum, const unsigned int *current_keyb, const unsigned int *old_keyb)
{
    if (current_keyb[keynum] && !old_keyb[keynum])
    {
        return true;
    }
    else
    {
        return false;
    }
}