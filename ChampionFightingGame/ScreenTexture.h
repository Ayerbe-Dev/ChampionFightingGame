#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCoord.h"

class ScreenTexture {
public:
	ScreenTexture();
	ScreenTexture(std::string path);
	ScreenTexture(std::vector<std::vector<char>> texture, int width = -1, int height = -1);
	ScreenTexture(const ScreenTexture& that);
	ScreenTexture& operator=(const ScreenTexture& that);
	~ScreenTexture();

	void init(std::string path);
	void init(std::vector<std::vector<char>> texture, int width = -1, int height = -1);
private:
	std::vector<std::vector<char>> texture;
};