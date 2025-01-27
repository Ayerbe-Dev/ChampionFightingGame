#include "BattleObject.h"
#include "ObjectManager.h"
#include "WindowManager.h"
#include "ShaderManager.h"

void BattleObject::play_camera_anim(std::string anim_kind, float anim_rate, float anim_frame) {
	Camera* camera = &window_manager->camera;
	camera->play_camera_anim(this, anim_kind, anim_rate, anim_frame, false);
}

void BattleObject::start_cinematic_sequence(std::string anim_kind, float anim_rate, float anim_frame, float world_brightness, bool dim_self, float world_rate) {
	Camera* camera = &window_manager->camera;
	camera->play_camera_anim(this, anim_kind, anim_rate, anim_frame, true);
	if (dim_self) {
		window_manager->dim_lights(world_brightness, nullptr);
	}
	else {
		window_manager->dim_lights(world_brightness, &shader);
	}
	object_manager->set_world_rate(this, world_rate);
}