#include "Fighter.h"
#include "RenderManager.h"
#include "ShaderManager.h"

void Fighter::start_cinematic_sequence(std::string anim_kind, float anim_rate, float anim_frame, float world_brightness, bool dim_self, float world_rate) {
	play_camera_anim(anim_kind, anim_rate, anim_frame);
	dim_world(world_brightness, dim_self);
	slow_world(world_rate);
	fighter_flag[FIGHTER_FLAG_RESET_CINEMATIC_ON_CAMERA_END] = true;
}

void Fighter::stop_cinematic_sequence() {
	Camera* camera = &render_manager->camera;
	if (camera->follow_id == id) {
		camera->anim_kind = nullptr;
		camera->following_players = true;
		camera->follow_id = -1;
		battle_object_manager->reset_world_rate(id);
		render_manager->dim_lights(1.0, nullptr);
		shader = shader_manager->get_shader("main", "main", "main", 0);
	}
}

void Fighter::play_camera_anim(std::string anim_kind, float rate, float frame) {
	fighter_flag[FIGHTER_FLAG_RESET_CINEMATIC_ON_CAMERA_END] = false;
	Camera* camera = &render_manager->camera;
	camera->play_camera_anim(id, anim_kind, rate, frame);
}

void Fighter::stop_camera_anim() {
	if (fighter_flag[FIGHTER_FLAG_RESET_CINEMATIC_ON_CAMERA_END]) {
		stop_cinematic_sequence();
	}
	else {
		Camera* camera = &render_manager->camera;
		if (camera->follow_id == id) {
			camera->anim_kind = nullptr;
			camera->following_players = true;
			camera->follow_id = -1;
		}
	}
}

void Fighter::slow_world(float world_rate) {
	battle_object_manager->set_world_rate(id, world_rate);
}

void Fighter::reset_world_rate() {
	battle_object_manager->reset_world_rate(id);
}

void Fighter::dim_world(float brightness, bool dim_self) {
	if (dim_self) {
		render_manager->dim_lights(brightness, nullptr);
	}
	else {
		render_manager->dim_lights(brightness, &shader);
	}
}

void Fighter::reset_world_brightness() {
	render_manager->dim_lights(1.0, nullptr);
	shader = shader_manager->get_shader("main", "main", "main", 0);
}