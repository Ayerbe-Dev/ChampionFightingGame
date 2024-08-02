#include "GameObject.h"
#include "ObjectManager.h"
#include "WindowManager.h"
#include "ShaderManager.h"

void GameObject::start_cinematic_sequence(std::string anim_kind, float anim_rate, float anim_frame, float world_brightness, bool dim_self, float world_rate) {
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

void GameObject::stop_cinematic_sequence() {
	Camera* camera = &window_manager->camera;
	if (camera->target == this) {
		camera->stop_camera_anim();
		object_manager->reset_world_rate(this);
		window_manager->dim_lights(1.0, nullptr);
		unsigned int flags = SHADER_FEAT_DIM_MUL;
		if (model.has_skeleton()) {
			flags |= SHADER_FEAT_BONES;
		}
		shader = shader_manager->get_shader("model", "model", "model", flags);
	}
}

void GameObject::play_camera_anim(std::string anim_kind, float rate, float frame) {
	Camera* camera = &window_manager->camera;
	camera->play_camera_anim(this, anim_kind, rate, frame, false);
}

void GameObject::stop_camera_anim() {
	Camera* camera = &window_manager->camera;
	if (camera->target == this) {
		camera->stop_camera_anim();
	}
}

void GameObject::slow_world(float world_rate) {
	object_manager->set_world_rate(this, world_rate);
}

void GameObject::reset_world_rate() {
	object_manager->reset_world_rate(this);
}

void GameObject::dim_world(float brightness, bool dim_self) {
	if (dim_self) {
		window_manager->dim_lights(brightness, nullptr);
	}
	else {
		window_manager->dim_lights(brightness, &shader);
	}
}

void GameObject::reset_world_brightness() {
	window_manager->dim_lights(1.0, nullptr);
	shader = shader_manager->get_shader_switch_features(shader, 0, SHADER_FEAT_DIM_MUL);
}