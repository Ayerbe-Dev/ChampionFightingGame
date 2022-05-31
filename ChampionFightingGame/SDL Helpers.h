#pragma once
#include <SDL/SDL.h>
#include <iostream>

SDL_Texture* loadSDLTexture(std::string file_path);
SDL_RendererFlip get_flip_kind(bool h_flipped, bool v_flipped);