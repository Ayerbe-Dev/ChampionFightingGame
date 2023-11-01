#include "Effect.h"
#include "Particle.h"
#include <fstream>
#include "RenderManager.h"
#include "ShaderManager.h"
#include "BattleObject.h"
#include "Fighter.h"
#include "utils.h"

EffectInfo::EffectInfo() {

}

EffectInfo::EffectInfo(std::string name, std::string dir) {
	this->name = name;
	this->dir = dir;
}

Effect::Effect() {
	duration = -1;
	shader = nullptr;
}

Effect::Effect(EffectInfo info) {
	init(info);
}

void Effect::init(EffectInfo info) {
	std::ifstream stream;
	stream.open(info.dir + "effect_info.yml");
	if (stream.fail()) {
		stream.close();
		return;
	}
	this->info = info;
	stream >> duration;

	std::string particle_name;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;
	glm::vec3 pos_frame;
	glm::vec3 rot_frame;
	glm::vec3 scale_frame;
	glm::vec4 rgba_frame;
	bool has_spritesheet;
	while (stream >> particle_name) {
		stream >> pos.x >> pos.y >> pos.z;
		stream >> rot.x >> rot.y >> rot.z;
		stream >> scale.x >> scale.y >> scale.z;
		stream >> rgba.x >> rgba.y >> rgba.z >> rgba.w;
		stream >> pos_frame.x >> pos_frame.y >> pos_frame.z;
		stream >> rot_frame.x >> rot_frame.y >> rot_frame.z;
		stream >> scale_frame.x >> scale_frame.y >> scale_frame.z;
		stream >> rgba_frame.x >> rgba_frame.y >> rgba_frame.z >> rgba_frame.w;
		stream >> has_spritesheet;
		OldParticle particle(info.dir + particle_name, pos, rot, scale, rgba, pos_frame, 
			rot_frame, scale_frame, rgba_frame);
		particles.push_back(particle);
	}
	ShaderManager* shader_manager = ShaderManager::get_instance();
	attach_shader(shader_manager->get_shader("effect", "effect", "", 0));
	shader->use();
}

void Effect::destroy() {
	for (int i = 0, max = particles.size(); i < max; i++) {
		particles[i].destroy();
	}
}

void Effect::attach_shader(Shader* shader) {
	this->shader = shader;
}

EffectInstance Effect::instantiate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba,
	GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var, float rate, float frame) {
	EffectInstance ret(this, pos, rot, scale, rgba, (GameObject*)game_object, bone_id, bone_offset, 
		pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);
	return ret;
}

EffectInstance::EffectInstance() {
	effect = nullptr;
	shader = nullptr;
	game_object = nullptr;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rgba = glm::vec4(1.0);
	bone_id = -1;
	bone_offset = glm::vec3(0.0);
	pos_frame = glm::vec3(0.0);
	rot_frame = glm::vec3(0.0);
	scale_frame = glm::vec3(0.0);
	rgba_frame = glm::vec4(0.0);
	final_pos = glm::vec3(0.0);
	final_rot = glm::vec3(0.0);
	final_scale = glm::vec3(1.0);
	final_rgba = glm::vec4(1.0);
	scale_vec = glm::vec3(1.0);
	flip = false;
	interp_var = nullptr;
	frame = 0.0;
	rate = 1.0;
	interpolating = false;
}

EffectInstance::EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba,
	GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var, float rate, float frame) {
	this->effect = effect;
	shader = effect->shader;
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->rgba = rgba;
	this->game_object = game_object;
	this->bone_id = bone_id;
	this->bone_offset = bone_offset;
	this->pos_frame = pos_frame;
	this->rot_frame = rot_frame;
	this->scale_frame = scale_frame;
	this->rgba_frame = rgba_frame;
	final_pos = glm::vec3(0.0);
	final_rot = glm::vec3(0.0);
	final_scale = glm::vec3(1.0);
	final_rgba = glm::vec4(1.0);
	flip = false;
	this->interp_var = interp_var;
	this->rate = rate;
	this->frame = frame;
	if (game_object != nullptr) {
		scale_vec = glm::vec3(
			WINDOW_WIDTH / (100 * game_object->scale.x),
			WINDOW_HEIGHT / (100 * game_object->scale.y),
			WINDOW_DEPTH / (100 * game_object->scale.z)
		);
		BattleObject* battle_object = static_cast<BattleObject*>(game_object);
		if (battle_object != nullptr && !battle_object->facing_right) {
			flip = true;
		}
	}
	else {
		scale_vec = glm::vec3(1.0);
	}
	interpolating = false;
}

bool EffectInstance::process() {
	if (interp_var != nullptr && *interp_var) {
		BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
		BattleObject* battle_object = static_cast<BattleObject*>(game_object);
		if (battle_object != nullptr) {
			frame += 0.2 / (float)(*interp_var) * battle_object_manager->get_world_rate(battle_object->id);
		}
		interpolating = true;
	}
	else {
		if (interpolating) {
			frame -= 0.2;
		}
		interpolating = false;
		frame += rate;
	}
	if (frame >= effect->duration) {
		return false;
	}
	final_pos = pos + (pos_frame * frame);
	final_rot = rot + (rot_frame * frame);
	final_scale = scale + (scale_frame * frame);
	final_rgba = rgba + (rgba_frame * frame);
	if (game_object != nullptr) {
		if (bone_id != -1) {
			BattleObject* battle_object = static_cast<BattleObject*>(game_object);
			if (battle_object != nullptr) {
				final_pos += battle_object->get_bone_position(bone_id, bone_offset);
				final_rot += battle_object->get_bone_rotation(bone_id);
			}
			else {
				final_pos += game_object->get_bone_position(bone_id, bone_offset);
				final_rot += game_object->get_bone_rotation(bone_id);
			}
		}
		else {
			final_pos += game_object->pos;
			final_rot += game_object->rot;
		}
	}

	return true;
}

void EffectInstance::render() {
	glDepthMask(GL_TRUE);
	shader->use();
	shader->set_int("f_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	for (int i = 0, max = effect->particles.size(); i < max; i++) {
		effect->particles[i].render(shader, final_pos, final_rot, final_scale, final_rgba, scale_vec, flip, frame);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}