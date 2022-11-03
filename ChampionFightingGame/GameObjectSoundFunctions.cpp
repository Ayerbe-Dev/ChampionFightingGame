#include "GameObject.h"
#include "SoundManager.h"
#include "SaveManager.h"
#include "utils.h"

void GameObject::play_se(std::string se) {
	Sound sound = sound_manager->get_sound(se);
	if (!sound.buffer) {
		return;
	}
	glm::vec3 sound_pos = pos / glm::vec3(
		WINDOW_WIDTH / (100 * scale.x),
		WINDOW_HEIGHT / (100 * scale.y),
		WINDOW_DEPTH / (100 * scale.z)
	);
	float volume_mul = SaveManager::get_instance()->get_game_setting("se_vol") / 100.0;
	sound_effects.push_back(0);
	alGenSources(1, &sound_effects.back());
	alSourcef(sound_effects.back(), AL_PITCH, 1);
	alSourcef(voice_clips.back(), AL_GAIN, (7.5 + sound.volume_mod) * volume_mul);
	alSource3f(sound_effects.back(), AL_POSITION, sound_pos.x, sound_pos.y, sound_pos.z);
	alSource3f(sound_effects.back(), AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei(sound_effects.back(), AL_LOOPING, AL_FALSE);
	alSourcei(sound_effects.back(), AL_BUFFER, sound.buffer);
	alSourcePlay(sound_effects.back());
}

void GameObject::play_vc(std::string vc) {
	Sound sound = sound_manager->get_sound(vc);
	if (!sound.buffer) {
		return;
	}
	glm::vec3 sound_pos = pos / glm::vec3(
		WINDOW_WIDTH / (100 * scale.x),
		WINDOW_HEIGHT / (100 * scale.y),
		WINDOW_DEPTH / (100 * scale.z)
	);
	float volume_mul = SaveManager::get_instance()->get_game_setting("vc_vol") / 100.0;
	voice_clips.push_back(0);
	alGenSources(1, &voice_clips.back());
	alSourcef(voice_clips.back(), AL_PITCH, 1);
	alSourcef(voice_clips.back(), AL_GAIN, (7.5 + sound.volume_mod) * volume_mul);
	alSource3f(voice_clips.back(), AL_POSITION, sound_pos.x, sound_pos.y, sound_pos.z);
	alSource3f(voice_clips.back(), AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei(voice_clips.back(), AL_LOOPING, AL_FALSE);
	alSourcei(voice_clips.back(), AL_BUFFER, sound.buffer);
	alSourcePlay(voice_clips.back());
}

void GameObject::pause_se(std::string se) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(se);
	ALint buffer;
	for (unsigned int& source : sound_effects) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourcePause(source);
			return;
		}
	}
}

void GameObject::pause_se_all() {
	for (unsigned int& source : sound_effects) {
		alSourcePause(source);
	}
}

void GameObject::pause_vc(std::string vc) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(vc);
	ALint buffer;
	for (unsigned int& source : voice_clips) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourcePause(source);
			return;
		}
	}
}

void GameObject::pause_vc_all() {
	for (unsigned int& source : voice_clips) {
		alSourcePause(source);
	}
}

void GameObject::resume_se(std::string se) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(se);
	ALint buffer;
	for (unsigned int& source : sound_effects) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourcePlay(source);
			return;
		}
	}
}

void GameObject::resume_se_all() {
	for (unsigned int& source : sound_effects) {
		alSourcePlay(source);
	}
}

void GameObject::resume_vc(std::string vc) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(vc);
	ALint buffer;
	for (unsigned int& source : voice_clips) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourcePlay(source);
			return;
		}
	}
}

void GameObject::resume_vc_all() {
	for (unsigned int& source : voice_clips) {
		alSourcePlay(source);
	}
}

void GameObject::stop_se(std::string se) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(se);
	ALint buffer;
	for (unsigned int& source : sound_effects) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourceStop(source);
			alDeleteSources(1, &source);
			sound_effects.remove(source);
			return;
		}
	}
}

void GameObject::stop_se_all() {
	for (unsigned int& source : sound_effects) {
		alSourceStop(source);
		alDeleteSources(1, &source);
	}
	sound_effects.clear();
}

void GameObject::stop_vc(std::string vc) {
	unsigned int buffer_id = sound_manager->get_sound_buffer(vc);
	ALint buffer;
	for (unsigned int& source : voice_clips) {
		alGetSourcei(source, AL_BUFFER, &buffer);
		if (buffer == buffer_id) {
			alSourceStop(source);
			alDeleteSources(1, &source);
			voice_clips.remove(source);
			return;
		}
	}
}

void GameObject::stop_vc_all() {
	for (unsigned int& source : voice_clips) {
		alSourceStop(source);
		alDeleteSources(1, &source);
	}
	voice_clips.clear();
}

void GameObject::load_sound(std::string name, std::string dir, float volume_mod) {
	sound_manager->load_sound(name, dir, volume_mod);
}

void GameObject::unload_sound(std::string name) {
	sound_manager->unload_sound(name);
}

void GameObject::unload_all_sounds() {
	sound_manager->unload_all_sounds();
}