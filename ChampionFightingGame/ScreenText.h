#pragma once
#include <iostream>
#include "Font.h"
#include "TextureCommon.h"
#include "TargetStructs.h"

//Needed:
//	- Pos/Rot
//	- Text, obviously
//	- Alpha
//	- Color
//	- Size? Might be covered under font...
//	- shit man is WorldText gonna be a thing later
//		- prob not, either way ST should still not just use ScreenTexture as a base the way old GT worked

class ScreenText {
public:
	ScreenText();

	ScreenText& init();

	ScreenText& update_text(std::string new_text);

	void render();

private:
#ifdef TEX_IMPL_MODE_VULKAN
	//TODO: However Fez handles Vulkan vert info, plop it here
#else
	unsigned int VAO;
	unsigned int VBO;
#endif
	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;

	TargetVar<unsigned char> alpha;
	TargetVar<glm::vec3> colormod;

	std::vector<TextureCoord> v_data_for_gpu; //We can modify this directly since textures won't support
	//flipping

	//That being said, text scrolling is going to require 2 tris per line (:
	
	int lines;

	Shader* shader;
	std::string text;
	unsigned int texture;
	Font* font; 
};