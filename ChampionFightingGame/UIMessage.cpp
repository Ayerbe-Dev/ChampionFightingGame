#include "UIMessage.h"

UIMessage::UIMessage() {
	active_condition = nullptr;
	active_duration = -1;
	font = nullptr;
	rgba = glm::vec4(0.0);
	border_rgbs = glm::vec4(0.0);
	fade_frames = 0;
}

void UIMessage::init(Font* font, std::string text, int active_duration, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	texture.init(*font, text, rgba, border_rgbs);
	texture.set_pos(glm::vec3(pos, 0.0f));
	this->rgba = rgba;
	this->border_rgbs = border_rgbs;
	this->font = font;
	this->active_duration = active_duration;
	this->fade_frames = fade_frames;
}

void UIMessage::init(Font* font, std::string text, bool* active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	texture.init(*font, text, rgba, border_rgbs);
	texture.set_pos(glm::vec3(pos, 0.0f));
	this->rgba = rgba;
	this->border_rgbs = border_rgbs;
	this->font = font;
	this->active_condition = active_condition;
	this->fade_frames = fade_frames;
}

void UIMessage::init(Font* font, std::string text, VBP active_condition, int fade_frames, glm::vec2 pos, glm::vec4 rgba, glm::vec4 border_rgbs) {
	texture.init(*font, text, rgba, border_rgbs);
	texture.set_pos(glm::vec3(pos, 0.0f));
	this->rgba = rgba;
	this->border_rgbs = border_rgbs;
	this->font = font;
	this->active_vector_condition = active_condition;
	this->fade_frames = fade_frames;
}

void UIMessage::update_text(std::string text) {
	texture.update_text(*font, text, rgba, border_rgbs);
}

void UIMessage::update_text(std::string text, int active_duration) {
	texture.update_text(*font, text, rgba, border_rgbs);
	this->active_duration = active_duration;
}

bool UIMessage::check_active() {
	if (active_duration == 0 
		|| active_vector_condition 
		|| (active_condition && *active_condition)) {
		texture.alpha.set_target_val(0, fade_frames);
		return false;
	}
	if (active_duration != -1) active_duration--;
	return true;
}

void UIMessage::render() {
	texture.process();
	texture.render();
}