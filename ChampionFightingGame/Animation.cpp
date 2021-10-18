using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <string>
#include <iostream>
#include <functional>
#include "Animation.h"

Animation::Animation() {};

SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer) {
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		cout << "Error loading image: " << IMG_GetError() << endl;
	}
	SDL_Texture *ret = SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface);
	return ret;
}

SDL_Rect getFrame(int frame, Animation* animation) {
	int width;
	int height;
	SDL_QueryTexture(animation->SPRITESHEET, NULL, NULL, &width, &height);
	SDL_Rect frame_rect;
	frame_rect.x = frame * animation->sprite_width;
	frame_rect.y = 0;
	frame_rect.w = animation->sprite_width;
	frame_rect.h = animation->sprite_height;
	return frame_rect;
}

void loadAnimation(Animation* animation, SDL_Renderer* renderer) {
	animation->SPRITESHEET = loadTexture((animation->path).c_str(), renderer);
}