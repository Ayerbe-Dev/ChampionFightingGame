#include "SDL Helpers.h"
#include <SDL/SDL_image.h>
#include "utils.h"

extern SDL_mutex* file_mutex;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

/// <summary>
/// Loads SDL textures. This should generally be avoided during normal gameplay as everything should be loaded beforehand.
/// </summary>
/// <param name="file_path">: File path from the game's directory</param>
/// <param name="delay">: Whether or not to wait 1 frame after running, for loading thread purposes. True by default, set to false if
/// using this function to load something with no loading screen.</param>
/// <returns> The loaded texture</returns>
SDL_Texture* loadSDLTexture(const char* file_path, bool delay) {
	SDL_LockMutex(file_mutex);
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		std::cout << "Error loading image: " << IMG_GetError() << "\n";
	}
	SDL_Texture* ret = SDL_CreateTextureFromSurface(g_renderer, image_surface);
	SDL_FreeSurface(image_surface);
	SDL_UnlockMutex(file_mutex);
	if (delay) {
		frameTimeDelay();
	}
	return ret;
}

/// <summary>
/// T H E   C Y C L E   O F   D E S T R U C T I O N   A N D   R E C R E A T I O N
/// </summary>
void refreshRenderer() {
	SDL_RenderClear(g_renderer);
	SDL_DestroyRenderer(g_renderer);
	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
}