#include "UIMessage.h"

UIMessage::UIMessage() {
	active_condition = nullptr;
	active_duration = -1;
	fade_frames = 0;
}

void UIMessage::init(Font* font, std::string text, int active_duration, int fade_frames, glm::vec2 pos, TextSpecifier spec) {
	this->ScreenText::init(font, text, spec);
	this->set_pos(glm::vec3(pos, 0.0f));
	this->active_duration = active_duration;
	this->fade_frames = fade_frames;
}

void UIMessage::init(Font* font, std::string text, bool* active_condition, int fade_frames, glm::vec2 pos, TextSpecifier spec) {
	this->ScreenText::init(font, text, spec);
	this->set_pos(glm::vec3(pos, 0.0f));
	this->active_condition = active_condition;
	this->fade_frames = fade_frames;
}

void UIMessage::init(Font* font, std::string text, VBP active_condition, int fade_frames, glm::vec2 pos, TextSpecifier spec) {
	this->ScreenText::init(font, text, spec);
	this->set_pos(glm::vec3(pos, 0.0f));
	this->active_vector_condition = active_condition;
	this->fade_frames = fade_frames;
}

void UIMessage::update_text(std::string text, int active_duration) {
	this->ScreenText::update_text(text);
	this->active_duration = active_duration;
}

bool UIMessage::check_active() {
	if (active_duration == 0 
	|| active_vector_condition 
	|| (active_condition && *active_condition)) {
		this->ScreenText::set_alpha(0, fade_frames);
		return false;
	}
	if (active_duration != -1) active_duration--;
	return true;
}