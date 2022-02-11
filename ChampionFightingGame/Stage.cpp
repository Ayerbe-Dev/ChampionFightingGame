#include "Stage.h"
extern RenderManager g_rendermanager;

Stage::Stage() {}

Stage::Stage(int stage_kind, string stage_name, vec3 origin) {
	this->stage_kind = stage_kind;
	this->origin = origin;
	g_rendermanager.stage_origin = origin;
	resource_dir = "resource/stage/" + stage_name + "/";

	default_music_kind = get_stage_music();
}

int Stage::get_stage_music() {
	switch (stage_kind) {
		case (STAGE_KIND_TRAINING_OLD): {
			return MUSIC_KIND_ATLAS_STAGE;
		} break;
		default: {
			return MUSIC_KIND_TRAINING_STAGE;
		} break;
	}
}