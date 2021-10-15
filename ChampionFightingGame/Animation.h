#pragma once
#include <SDL.h>
#include <string>
#include <functional>
struct Animation {
	SDL_Texture* SPRITESHEET;
	std::string name;
	std::string path;
	int length;
	int sprite_height;
	int sprite_width;
	int faf;

	Animation();
};

class RoyScript {
public:
	string name;
	void (RoyScript::* move_script)();

	RoyScript();
	RoyScript(string name, function<void()> move_script);
};

SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer);
SDL_Rect getFrame(int frame, Animation* animation);
void loadAnimation(Animation* animation, SDL_Renderer* renderer);


extern Animation ANIM_TABLE[60][2];
