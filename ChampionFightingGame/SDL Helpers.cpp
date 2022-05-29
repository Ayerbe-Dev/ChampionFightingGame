#include "SDL Helpers.h"
#include <SDL/SDL_image.h>
#include "utils.h"
#include "RenderManager.h"
#include <mutex>

extern std::mutex file_mutex;

/// <summary>
/// Loads SDL textures. This should generally be avoided during normal gameplay as everything should be loaded beforehand.
/// </summary>
/// <param name="file_path">: File path from the game's directory</param>
/// <param name="delay">: Whether or not to wait 1 frame after running, for loading thread purposes. True by default, set to false if
/// using this function to load something with no loading screen.</param>
/// <returns> The loaded texture</returns>
SDL_Texture* loadSDLTexture(const char* file_path, bool delay) {
	file_mutex.lock();
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		std::cout << "Error loading image: " << IMG_GetError() << "\n";
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(RenderManager::get_instance()->sdl_renderer, image_surface);
	SDL_FreeSurface(image_surface);
	file_mutex.unlock();
	if (delay) {
		wait_ms();
	}
	return ret;
}