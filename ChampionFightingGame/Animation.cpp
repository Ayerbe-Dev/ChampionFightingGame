using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include <string>
#include <iostream>
#include <functional>
#include "Animation.h"

Animation::Animation() {}
Animation::Animation(std::string dir, int length, int width, int height) :
	ANIMATION_DIR{ dir }, length{ length - 1 }, sprite_height{ height }, sprite_width{ width } {}

SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer) {
	SDL_Surface* image_surface = IMG_Load(file_path);
	if (image_surface == NULL) {
		cout << "Error loading image: " << IMG_GetError() << endl;
	}
	return SDL_CreateTextureFromSurface(renderer, image_surface);
	SDL_FreeSurface(image_surface); // haha no more memory leaks
}

SDL_Rect getFrame(int frame, Animation* animation) {
	SDL_Rect frame_rect;
	frame_rect.x = frame * animation->sprite_width;
	frame_rect.y = 0;
	frame_rect.h = animation->sprite_height;
	frame_rect.w = animation->sprite_width;
	return frame_rect;
}

void loadAnimation(Animation* animation, SDL_Renderer* renderer) {
	animation->SPRITESHEET = loadTexture((animation->ANIMATION_DIR).c_str(), renderer);
}

Animation ANIM_TABLE[60][2];