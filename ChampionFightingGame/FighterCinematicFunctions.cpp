#include "Fighter.h"
#include "RenderManager.h"

void Fighter::play_camera_anim(std::string anim_name, float rate, float world_brightness, float world_rate) {
	if (render_manager->camera.following_players) {
		if (world_rate != battle_object_manager->world_rate) {
			fighter_float[FIGHTER_FLOAT_PREV_WORLD_RATE] = battle_object_manager->world_rate;
			battle_object_manager->set_world_rate(id, world_rate);
		}
		if (world_brightness != render_manager->brightness_mul) {
			fighter_float[FIGHTER_FLOAT_PREV_WORLD_BRIGHTNESS] = render_manager->brightness_mul;
			render_manager->dim_lights(world_brightness);
			render_manager->undim_shader(&shader);
		}
		if (anim_name != "") {
			render_manager->camera.play_camera_anim(id, anim_name, rate);
		}
	}
}

void Fighter::end_camera_anim() {
	battle_object_manager->set_world_rate(id, fighter_float[FIGHTER_FLOAT_PREV_WORLD_RATE]);
	render_manager->dim_lights(fighter_float[FIGHTER_FLOAT_PREV_WORLD_BRIGHTNESS]);
}

void Fighter::slow_world(float world_rate) {
	battle_object_manager->set_world_rate(id, 1.0);
}

void Fighter::reset_world_rate() {
	battle_object_manager->reset_world_rate(id);
}
