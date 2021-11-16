#include "Stage.h"

Stage::Stage() {}

Stage::Stage(int stage_kind, string stage_name, int music_index) {
	this->stage_kind = stage_kind;
	resource_dir = "resource/stage/" + stage_name + "/";

	default_music_kind = music_index; //By default, the song at this index = The stage music
	
	string background_texture = this->resource_dir + "background.png";
	pBackgroundTexture = loadTexture((background_texture).c_str());
}