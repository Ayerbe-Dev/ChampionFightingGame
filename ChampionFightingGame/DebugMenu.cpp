#include <iostream>
#include <SDL.h>
#include <stdbool.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include "utils.h"
#include "DebugMenu.h"

int debugMenu(int iLastMenu, SDL_Renderer *pRenderer)
{

    const Uint8 *keyboard_state;
    int select = 0;
    std::ostringstream lastString;
    SDL_Texture *option_texts[6];
    int option_surfaces[6];
    SDL_Rect dest;
    bool debugging = true;
    bool bPressedDown = false;
    bool bPressedUp = false;
    SDL_Rect selectRect{0, 0, 100, 700};

    if (TTF_Init() < 0)
    {
        printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
    }

    TTF_Font *font;

    font = TTF_OpenFont("FiraCode-Regular.ttf", 24);
    if (!font)
    {
        printf("Failed to load font:  %s\n", TTF_GetError());
    }

    SDL_Surface *text;
    // Set color to black
    SDL_Color color = {255, 255, 255};
    text = TTF_RenderText_Solid(font, "menu entry int\nback", color);

    if (!text)
    {
        printf("Failed to render text:  %s\n", TTF_GetError());
    }

    // SDL_Texture *text_texture;

    // text_texture = SDL_CreateTextureFromSurface(pRenderer, text);

    option_surfaces[0] = 280;
    lastString << "Menu Call [" << iLastMenu << "] 'a' to select";
    option_texts[0] = newFontTexture(lastString.str(), pRenderer, font);

    option_surfaces[1] = 50;
    option_texts[1] = newFontTexture("CLOSE", pRenderer, font);

    option_surfaces[2] = 50;
    option_texts[2] = newFontTexture("GAME", pRenderer, font);

    option_surfaces[3] = 60;
    option_texts[3] = newFontTexture("MENU", pRenderer, font);

    option_surfaces[4] = 60;
    option_texts[4] = newFontTexture("CCS", pRenderer, font);

    option_surfaces[5] = 120;
    option_texts[5] = newFontTexture("DEBUG (this)", pRenderer, font);

    printf("DEBUG");
    while (debugging)
    {
        for (int i = 0; i <= GAME_STATE_MAX; i++)
        {
            dest = {100, 26 * i, option_surfaces[i], 25}; // I am so lazy oml
            SDL_RenderCopy(pRenderer, option_texts[i], nullptr, &dest);
        }

        selectRect = {0, 0, 100, 700};
        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(pRenderer, &selectRect);

        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        selectRect = {75, (select + 1) * 26 + 5, 15, 15};
        SDL_RenderFillRect(pRenderer, &selectRect);

        SDL_RenderPresent(pRenderer);
        SDL_PumpEvents();

        keyboard_state = SDL_GetKeyboardState(nullptr);
        if (keyboard_state[SDL_SCANCODE_DOWN] && select < GAME_STATE_MAX - 1)
        {
            select++;
        }
        else if (keyboard_state[SDL_SCANCODE_UP] && select > 0)
        {
            select--;
        }
        else if (keyboard_state[SDL_SCANCODE_A])
        {
            SDL_Delay(200);
            return select;
        }
        SDL_Delay(1000 / 10);
    }
}

SDL_Texture *newFontTexture(std::string text, SDL_Renderer *pRenderer, TTF_Font *font)
{
    SDL_Surface *textSurface;
    // Set color to black
    SDL_Color color = {255, 255, 255};
    textSurface = TTF_RenderText_Solid(font, text.c_str(), color);

    if (!textSurface)
    {
        printf("Failed to render text:  %s\n", TTF_GetError());
    }
    return SDL_CreateTextureFromSurface(pRenderer, textSurface);
}