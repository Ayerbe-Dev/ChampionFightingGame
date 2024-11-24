#pragma once
#include "ScreenText.h"
#include "VectorBoolPointer.h"

class UIMessage : public ScreenText {
public:
	UIMessage();

	void init(Font* font, std::string text, int active_duration, int fade_frames, glm::vec2 pos, TextSpecifier spec);
	void init(Font* font, std::string text, bool* active_condition, int fade_frames, glm::vec2 pos, TextSpecifier spec);
	void init(Font* font, std::string text, VBP active_condition, int fade_frames, glm::vec2 pos, TextSpecifier spec);
	void update_text(std::string text, int active_duration);
	bool check_active();
private:
	int active_duration;
	bool* active_condition;
	VBP active_vector_condition;
	int fade_frames;
};