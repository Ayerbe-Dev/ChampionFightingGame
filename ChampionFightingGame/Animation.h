#pragma once
#include <SDL.h>
#include <string>
struct Animation {
	SDL_Texture* SPRITESHEET;
	std::string ANIMATION_DIR;
	int length;
	int sprite_height;
	int sprite_width;
	Animation();
	Animation(std::string dir, int length, int width, int height); 
};

SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer);
SDL_Rect getFrame(int frame, Animation* animation);
void loadAnimation(Animation* animation, SDL_Renderer* renderer);


extern Animation TEST_IDLE_ANIMATION;
extern Animation TEST_WALK_ANIMATION;
