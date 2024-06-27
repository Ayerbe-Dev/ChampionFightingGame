#pragma once
#include "GameTexture.h"
#include "VectorBoolPointer.h"

class UIMessage {
public:
	UIMessage();

	void init(Font* font, std::string text, int active_duration, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs);
	void init(Font* font, std::string text, bool* active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs);
	void init(Font* font, std::string text, VBP active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs);
	void update_text(std::string text);
	void update_text(std::string text, int active_duration);
	bool check_active();
	void render();

	GameTexture texture;
private:
	Font* font;
	glm::vec4 rgba; 
	glm::vec4 border_rgbs;

	int active_duration;
	bool* active_condition;
	VBP active_vector_condition;
	int fade_frames;
};