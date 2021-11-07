#include "Stage.h"

Stage::Stage() {}

Stage::Stage(string stage_id) {
	this->stage_id = stage_id;
	this->resource_dir = "resource/stage/" + stage_id + "/";
	string background_texture = this->resource_dir + "/background.png";
	this->pBackgroundTexture = loadTexture((background_texture).c_str());
}