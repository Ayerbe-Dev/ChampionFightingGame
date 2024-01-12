#include "SDL Helpers.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_render.h>
#include "utils.h"
#include "RenderManager.h"

/// <summary>
/// Loads SDL textures. This should generally be avoided during normal gameplay as everything should be loaded beforehand.
/// </summary>
/// <param name="file_path">: File path from the game's directory</param>
/// <returns> The loaded texture</returns>
SDL_Texture* loadSDLTexture(std::string file_path) {
	SDL_Surface* image_surface = IMG_Load(file_path.c_str());
	if (image_surface == NULL) {
		std::cout << "Error loading image: " << IMG_GetError() << "\n";
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(RenderManager::get_instance()->sdl_renderer, image_surface);
	SDL_FreeSurface(image_surface);
	return ret;
}

SDL_RendererFlip get_flip_kind(bool h_flipped, bool v_flipped) {
	if (h_flipped == v_flipped) {
		if (h_flipped) {
			return SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		else {
			return SDL_FLIP_NONE;
		}
	}
	else if (h_flipped) {
		return SDL_FLIP_HORIZONTAL;
	}
	else {
		return SDL_FLIP_VERTICAL;
	}
}