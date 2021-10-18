#include "UI.h"

HealthBar::HealthBar() {}
HealthBar::HealthBar(SDL_Renderer* renderer, FighterInstance* fighter_instance) {
	this->fighter_instance = fighter_instance;
	this->bar_texture = loadTexture("resource/ui/game/hp/bar.png", renderer);
	this->health_texture = loadTexture("resource/ui/game/hp/health.png", renderer);
	this->max_health = fighter_instance->stats.health;
	if (fighter_instance->id == 0) {
		this->health_rect.x = 0;
		this->health_rect.y = 0;
		this->health_rect.w = 400;
		this->health_rect.h = 50;
		this->bar_rect.x = 0;
		this->bar_rect.y = 0;
		this->bar_rect.w = 400;
		this->bar_rect.h = 50;
	}
	else {
		int width;
		int height;
		SDL_QueryTexture(this->health_texture, NULL, NULL, &width, &height);
		this->health_rect.x = WINDOW_WIDTH - width;
		this->health_rect.y = 0;
		this->health_rect.w = 400;
		this->health_rect.h = 50;
		this->bar_rect.x = WINDOW_WIDTH - width;
		this->bar_rect.y = 0;
		this->bar_rect.w = 400;
		this->bar_rect.h = 50;
	}
}

PlayerIndicator::PlayerIndicator() {}
PlayerIndicator::PlayerIndicator(SDL_Renderer* renderer, FighterInstance* fighter_instance, string nametag) {
	this->fighter_instance = fighter_instance;
	this->nametag = nametag;
	string resource_dir = "resource/ui/game/tag/";
	if (fighter_instance->id == 0) {
		resource_dir += "p1_tag";
	}
	else {
		resource_dir += "p2_tag";
	}
	if (nametag == "") {
		resource_dir += "_no_nametag";
	}
	resource_dir += ".png";
	const char* file_dir = resource_dir.c_str();
	this->texture = loadTexture(file_dir, renderer);
	this->indicator_rect.x = 0;
	this->indicator_rect.y = 0;
	this->indicator_rect.w = 92;
	this->indicator_rect.h = 92;
}

Timer::Timer() {}
Timer::Timer(SDL_Renderer* renderer, int remaining) {
	this->remaining = remaining;
	string resource_dir = "resource/ui/game/timer/";
	if (remaining == -1) {
		resource_dir += "infinite.png";
	}
	else {
		resource_dir += "infinite.png"; //todo: Make a blank version of the timer with no infinite symbol
	}
	const char* file_dir = resource_dir.c_str();
	this->texture = loadTexture(file_dir, renderer);
	this->timer_rect.x = WINDOW_WIDTH / 2 - (92 / 2);
	this->timer_rect.y = 0;
	this->timer_rect.w = 92;
	this->timer_rect.h = 92;
};