#include "Effect.h"
#include "Particle.h"
#include <fstream>
#include "RenderManager.h"
#include "BattleObject.h"
#include "Fighter.h"

EffectInfo::EffectInfo() {

}

EffectInfo::EffectInfo(std::string name, std::string dir) {
	this->name = name;
	this->dir = dir;
}

Effect::Effect() {
	duration = -1;
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
		Particle particle(info.dir + particle_name + ".png", pos, rot, scale, rgba, pos_frame, 
			rot_frame, scale_frame, rgba_frame);
		if (has_spritesheet) {
			particle.load_spritesheet(info.dir + particle_name + ".yml");
		}
		particles.push_back(particle);
	}
	RenderManager* render_manager = RenderManager::get_instance();
	attach_shader(&render_manager->default_effect_shader);
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
	BattleObject* battle_object, int bone_id, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	EffectInstance ret(this, pos, rot, scale, rgba, battle_object, bone_id, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
	return ret;
}

EffectInstance::EffectInstance() {
	effect = nullptr;
	shader = nullptr;
	battle_object = nullptr;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rgba = glm::vec4(1.0);
	pos_frame = glm::vec3(0.0);
	rot_frame = glm::vec3(0.0);
	scale_frame = glm::vec3(0.0);
	rgba_frame = glm::vec4(0.0);
	final_pos = glm::vec3(0.0);
	final_rot = glm::vec3(0.0);
	final_scale = glm::vec3(1.0);
	final_rgba = glm::vec4(1.0);
	frame = 0.0;
	rate = 1.0;
	bone_id = -1;
}

EffectInstance::EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba,
	BattleObject* battle_object, int bone_id, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	this->effect = effect;
	shader = effect->shader;
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->rgba = rgba;
	this->battle_object = battle_object;
	this->bone_id = bone_id;
	this->pos_frame = pos_frame;
	this->rot_frame = rot_frame;
	this->scale_frame = scale_frame;
	this->rgba_frame = rgba_frame;
	final_pos = glm::vec3(0.0);
	final_rot = glm::vec3(0.0);
	final_scale = glm::vec3(1.0);
	final_rgba = glm::vec4(1.0);
	this->rate = rate;
	this->frame = frame;
	if (battle_object != nullptr) {
		scale_vec = glm::vec3(
			WINDOW_WIDTH / (100 * battle_object->scale.x),
			WINDOW_HEIGHT / (100 * battle_object->scale.y),
			WINDOW_DEPTH / (100 * battle_object->scale.z)
		);
	}
	else {
		scale_vec = glm::vec3(0.05);
	}
}

bool EffectInstance::process() {
	frame += rate;
	if (frame >= effect->duration) {
		return false;
	}

	return true;
}

void EffectInstance::render() {
	glDepthMask(GL_TRUE);
	shader->use();
	shader->set_int("f_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	RenderManager::get_instance()->update_shader_cam(shader);
	final_pos = pos + (pos_frame * frame);
	final_rot = rot + (rot_frame * frame);
	final_scale = scale + (scale_frame * frame);
	final_rgba = rgba + (rgba_frame * frame);
	if (battle_object != nullptr) {
		if (bone_id != -1) {
			final_pos += battle_object->get_bone_position(bone_id);
			final_rot += battle_object->get_bone_rotation(bone_id);
		}
		else {
			final_pos += battle_object->pos;
		}
		final_rot += battle_object->rot;
	}
	for (int i = 0, max = effect->particles.size(); i < max; i++) {
		effect->particles[i].render(shader, final_pos, final_rot, final_scale, final_rgba, scale_vec, frame);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}