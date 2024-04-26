#include "SoundPlayer.h"
#include "GameManager.h"
#include "SaveManager.h"
#include "Sndlst.h"

SoundPlayer::SoundPlayer() {
	sound_manager = SoundManager::get_instance();
	sound_manager->register_sound_player(this);
	object_pos = nullptr;
	sound_end = false;
}

SoundPlayer::~SoundPlayer() {
	sound_manager->unregister_sound_player(this);
}

void SoundPlayer::init(glm::vec3* object_pos) {
	this->object_pos = object_pos;
}

bool SoundPlayer::is_sound_end() {
	return sound_end;
}

void SoundPlayer::process_sound() {
	sound_end = false;
	std::list<SoundInstance>::iterator source = sound_instances.begin();
	while (source != sound_instances.end()) { //This pass handles all sounds that still have yet to be played.
		//(We push new sounds to the front of our list so whenever we see a source has already been
		//played, we can be sure that we don't have any more un-played sounds to worry about)
		if (source->buffered) break;
		source->start(object_pos);
		source++;
	}
	while (source != sound_instances.end()) { //This pass handles all sounds which are already playing, I.E.
		//all sounds that weren't accounted for by the last loop.
		ALint state;
		alGetSourcei(source->al_source, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED) {
			alDeleteSources(1, &(source->al_source));
			source = sound_instances.erase(source);
			sound_end = true;
			continue;
		}
		source++;
	}
	if (reserved_sound.active) {
		if (reserved_sound.buffered) {
			ALint state;
			alGetSourcei(reserved_sound.al_source, AL_SOURCE_STATE, &state);
			if (state == AL_STOPPED) {
				alDeleteSources(1, &reserved_sound.al_source);
				reserved_sound = SoundInstance();
				sound_end = true;
			}
		}
		else {
			reserved_sound.start(object_pos);
			reserved_sound.buffered = true;
		}
	}
}

void SoundPlayer::play_sound(std::string name, float volume_mod) {
	if (sound_data.contains(name)) {
		sound_instances.push_front(sound_data[name].instantiate(volume_mod));
	}
}

void SoundPlayer::pause_sound(std::string name) {
	unsigned int buffer_id = sound_data[name].buffer;
	for (SoundInstance& instance : sound_instances) {
		if (instance.sound_source == buffer_id) {
			alSourcePause(instance.al_source);
			return;
		}
	}
}

void SoundPlayer::pause_sound_all() {
	for (SoundInstance& instance : sound_instances) {
		alSourcePause(instance.al_source);
	}
}

void SoundPlayer::resume_sound(std::string name) {
	unsigned int buffer_id = sound_data[name].buffer;
	for (SoundInstance& instance : sound_instances) {
		if (instance.sound_source == buffer_id) {
			alSourcePlay(instance.al_source);
			return;
		}
	}
}

void SoundPlayer::resume_sound_all() {
	for (SoundInstance& instance : sound_instances) {
		alSourcePlay(instance.al_source);
	}
}

void SoundPlayer::stop_sound(std::string name) {
	unsigned int buffer_id = sound_data[name].buffer;
	for (std::list<SoundInstance>::iterator source = sound_instances.begin(); source != sound_instances.end(); source++) {
		if (source->sound_source == buffer_id) {
			alSourceStop(source->al_source);
			alDeleteBuffers(1, &(source->al_source));
			sound_instances.erase(source);
			return;
		}
	}
}

void SoundPlayer::stop_sound_all() {
	for (SoundInstance& instance : sound_instances) {
		alSourceStop(instance.al_source);
		alDeleteBuffers(1, &instance.al_source);
	}
	sound_instances.clear();
}

void SoundPlayer::play_reserved_sound(std::string name, float volume_mod) {
	if (sound_data.contains(name)) {
		reserved_sound = sound_data[name].instantiate(volume_mod);
	}
}

void SoundPlayer::pause_reserved_sound() {
	if (reserved_sound.al_source) {
		alSourcePause(reserved_sound.al_source);
	}
}

void SoundPlayer::resume_reserved_sound() {
	if (reserved_sound.al_source) {
		alSourcePlay(reserved_sound.al_source);
	}
}

void SoundPlayer::stop_reserved_sound() {
	if (reserved_sound.al_source) {
		alSourceStop(reserved_sound.al_source);
		alDeleteSources(1, &reserved_sound.al_source);
		reserved_sound = SoundInstance();
	}
}

void SoundPlayer::load_sound_list(std::string list, std::string dir) {
	std::ifstream sound_stream;
	std::string name;
	std::string file;
	sound_stream.open(dir + "/" + list + "/" + list + "_list.sndlst");
	if (sound_stream.fail()) {
		sound_stream.close();
		GameManager::get_instance()->add_crash_log(
			"Couldn't find file " + dir + "/" + list + "/" + list + "_list.sndlst"
		);
	}
	else {
		while (!sound_stream.eof()) {
			parse_sndlst_entry(sound_stream, name, file);
			if (name == "") {
				break;
			}
			load_sound(name, dir + "/" + list + "/" + file);
		}
		sound_stream.close();
	}
}

void SoundPlayer::load_sound(std::string name, std::string dir) {
	sound_data[name] = sound_manager->get_sound(dir);
}

void SoundPlayer::unload_sound(std::string name) {
	if (sound_data.contains(name)) {
		sound_manager->unuse_sound(sound_data[name].dir);
		sound_data.erase(name);
	}
}

void SoundPlayer::unload_all_sounds() {
	for (const auto& sound : sound_data) {
		sound_manager->unuse_sound(sound.second.dir);
		sound_data.erase(sound.first);
	}
}