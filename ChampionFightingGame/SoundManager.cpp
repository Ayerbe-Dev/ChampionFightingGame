#include "SoundManager.h"
#include "SaveManager.h"
#include "SDL/SDL_audio.h"
#include "OpenAL/alc.h"
#include "OpenAL/al.h"
#include "utils.h"
#include "GameObject.h"

SoundManager::SoundManager() {
	al_device = alcOpenDevice(nullptr);
	al_context = alcCreateContext(al_device, nullptr);
	alcMakeContextCurrent(al_context);
}

void SoundManager::process_sounds() {
	for (int i = 0, max = objects.size(); i < max; i++) {
		objects[i]->process_sound();
	}
	ALint state;
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		update_stream(*music);
		alGetSourcei(music->source, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED) {
			alDeleteSources(1, &(music->source));
			if (active_music.size() != 1) {
				music = active_music.erase(music);
			}
			else {
				active_music.erase(music);
			}
		}
	}
}

void SoundManager::play_music(std::string name) {
	if (!music_map.contains(name)) {
		std::cerr << "Music " << name << " not loaded!\n";
		return;
	}
	float volume_mul = SaveManager::get_instance()->get_game_setting("music_vol") / 100.0;
	Music& music = music_map[name];
	active_music.push_back(MusicInstance());
	unsigned int& source = active_music.back().source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, (0.5 + music.volume_mod) * volume_mul);
	alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
	alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	active_music.back().name = name;
	active_music.back().curr_track = 0;
	active_music.back().cursor = music.num_buffers * BUFFER_SIZE;
	unsigned int track_pos = 0;
	while (track_pos != active_music.back().cursor) {
		if (music.tracks[active_music.back().curr_track].data.size() < active_music.back().cursor - track_pos) {
			track_pos += music.tracks[active_music.back().curr_track].data.size();
			active_music.back().curr_track = music.tracks[active_music.back().curr_track].next_track;
		}
		else {
			break;
		}
	}
	active_music.back().cursor -= track_pos;
	
	alSourceQueueBuffers(source, music.num_buffers, &music.buffers[0]);
	alSourcePlay(source);
}

void SoundManager::pause_music(std::string name) {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		if (music->name == name) {
			alSourcePause(music->source);
			return;
		}
	}
}

void SoundManager::pause_music_all() {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		alSourcePause(music->source);
	}
}

void SoundManager::resume_music(std::string name) {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		if (music->name == name) {
			alSourcePlay(music->source);
			return;
		}
	}
}

void SoundManager::resume_music_all() {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		alSourcePlay(music->source);
	}
}

void SoundManager::stop_music(std::string name) {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		if (music->name == name) {
			alSourceStop(music->source);
			alDeleteSources(1, &music->source);
			active_music.erase(music);
			return;
		}
	}
}

void SoundManager::stop_music_all() {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		alSourceStop(music->source);
		alDeleteSources(1, &music->source);
	}
	active_music.clear();
}

void SoundManager::pause_vc_all() {
	for (int i = 0, max = objects.size(); i < max; i++) {
		objects[i]->pause_vc_all();
	}
}

void SoundManager::pause_se_all() {
	for (int i = 0, max = objects.size(); i < max; i++) {
		objects[i]->pause_se_all();
	}
}

void SoundManager::resume_vc_all() {
	for (int i = 0, max = objects.size(); i < max; i++) {
		objects[i]->resume_vc_all();
	}
}

void SoundManager::resume_se_all() {
	for (int i = 0, max = objects.size(); i < max; i++) {
		objects[i]->resume_se_all();
	}
}

void SoundManager::register_game_object(GameObject* game_object) {
	objects.push_back(game_object);
}

void SoundManager::clear_game_objects() {
	objects.clear();
}

Sound SoundManager::get_sound(std::string name) {
	if (sound_map.contains(name)) {
		return sound_map[name];
	}
	std::cerr << "Sound " << name << " not loaded!\n";
	return Sound();
}

unsigned int SoundManager::get_sound_buffer(std::string name) {
	if (sound_map.contains(name)) {
		return sound_map[name].buffer;
	}
	std::cerr << "Sound " << name << " not loaded!\n";
	return 0;
}

void SoundManager::load_sound(std::string name, std::string dir, float volume_mod) {
	if (sound_map.contains(name)) {
		return;
	}
	sound_map[name] = Sound(dir, volume_mod);
}

void SoundManager::load_music(std::string name, std::string dir, float volume_mod) {
	if (music_map.contains(name)) {
		return;
	}
	music_map[name] = Music(dir, volume_mod);
}

void SoundManager::unload_sound(std::string name) {
	if (sound_map.contains(name)) {
		alDeleteBuffers(1, &sound_map[name].buffer);
		sound_map.erase(name);
	}
}

void SoundManager::unload_music(std::string name) {
	if (music_map.contains(name)) {
		alDeleteBuffers(music_map[name].num_buffers, &music_map[name].buffers[0]);
		music_map.erase(name);
	}
}

void SoundManager::unload_all_sounds() {
	for (const auto& sound : sound_map) {
		alDeleteBuffers(1, &sound.second.buffer);
	}
	sound_map.clear();
}

void SoundManager::unload_all_music() {
	for (const auto& music : music_map) {
		alDeleteBuffers(music.second.num_buffers, &music.second.buffers[0]);
	}
	music_map.clear();
}

SoundManager* SoundManager::instance = nullptr;
SoundManager* SoundManager::get_instance() {
	if (instance == nullptr) {
		instance = new SoundManager;
	}
	return instance;
}

void SoundManager::destroy_instance() {
	alcDestroyContext(al_context);
	alcCloseDevice(al_device);
	unload_all_sounds();
	unload_all_music();
	if (instance != nullptr) {
		delete instance;
	}
}

void SoundManager::update_stream(MusicInstance& music_instance) {
	ALint buffers_processed = 0;
	Music& music = music_map[music_instance.name];
	for (alGetSourcei(music_instance.source, AL_BUFFERS_PROCESSED, &buffers_processed); buffers_processed > 0; buffers_processed--) {
		ALuint buffer;
		alSourceUnqueueBuffers(music_instance.source, 1, &buffer);
		std::string data;
		data.resize(BUFFER_SIZE);
		std::memset(data.data(), 0, BUFFER_SIZE); //Set the data to be empty
		std::size_t copied_size = 0;
		std::string track_data = music.tracks[music_instance.curr_track].data;
		while (copied_size != BUFFER_SIZE) { //Until we've filled the entire buffer
			std::size_t copy_size = BUFFER_SIZE - copied_size; //Copy size is however much we have left
			//to fill
			if (music_instance.cursor + copy_size > track_data.size()) { //If filling that amount will
				//bring us past the end of a track, just fill up until the end of the track
				copy_size = track_data.size() - music_instance.cursor;
			}
			std::memcpy(&data[copied_size], &track_data[music_instance.cursor], copy_size); //Copy our
			//track, from the cursor to either the buffer size or its end, into our string starting from
			//the position of the data we already copied
			music_instance.cursor += copy_size; //Then move our cursor forward

			if (copy_size != BUFFER_SIZE - copied_size) { //If copy_size was modified, that means we
				//hit the end of the track
				music_instance.curr_track = music.tracks[music_instance.curr_track].next_track;
				track_data = music.tracks[music_instance.curr_track].data;
				music_instance.cursor = 0;
			}
			copied_size += copy_size; //Now to increment copied_size
		}

		//Data should now be entirely filled

		alBufferData(buffer, music.format, data.c_str(), BUFFER_SIZE, music.sample_rate);
		alSourceQueueBuffers(music_instance.source, 1, &buffer);
	}
}