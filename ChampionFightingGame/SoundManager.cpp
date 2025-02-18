#include "SoundManager.h"
#include "SaveManager.h"
#include "OpenAL/alc.h"
#include "OpenAL/al.h"
#include "utils.h"
#include "SoundPlayer.h"

SoundResource::SoundResource() {
	user_count = 0;
}

MusicResource::MusicResource() {
	user_count = 0;
}

SoundManager::SoundManager() {
	al_device = alcOpenDevice(nullptr);
	al_context = alcCreateContext(al_device, nullptr);
	alcMakeContextCurrent(al_context);
}

void SoundManager::process_sounds() {
	for (SoundPlayer* sound_player : sound_players) {
		sound_player->process_sound();
	}
	ALint state;
	for (std::list<MusicInstance>::iterator music = active_music.begin(); music != active_music.end(); music++) {
		alGetSourcei(music->source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			update_stream(*music);
		}
		else if (state == AL_STOPPED) {
			alDeleteSources(1, &(music->source));
			active_music.erase(music);
		}
	}
}

MusicInstance* SoundManager::play_music(std::string dir, float volume_mod) {
	if (!music_map.contains(dir)) {
		std::cerr << "Music " << dir << " not loaded!\n";
		return nullptr;
	}
	float volume_mul = SaveManager::get_instance()->get_game_setting("music_vol") / 100.0;
	Music& music = music_map[dir].music;
	active_music.push_back(MusicInstance());
	unsigned int& source = active_music.back().source;
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, volume_mod + volume_mul);
	alSource3f(source, AL_POSITION, 0.0, 0.0, 0.0);
	alSource3f(source, AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei(source, AL_LOOPING, AL_FALSE);
	active_music.back().dir = dir;
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
	return &active_music.back();
}

void SoundManager::pause_music(std::string name) {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		if (music->dir == name) {
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
		if (music->dir == name) {
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
		if (music->dir == name) {
			alSourceStop(music->source);
			for (unsigned int i = 0; i < music_map[name].music.num_buffers; i++) {
				unsigned int buffer;
				alSourceUnqueueBuffers(music->source, 1, &buffer);
			}
			alDeleteSources(1, &music->source);
			music_map[name].music.reset_buffers();
			active_music.erase(music);
			return;
		}
	}
}

void SoundManager::stop_music_all() {
	for (std::list<MusicInstance>::iterator music = active_music.begin(), max = active_music.end(); music != max; music++) {
		alSourceStop(music->source);
		for (unsigned int i = 0; i < music_map[music->dir].music.num_buffers; i++) {
			unsigned int buffer;
			alSourceUnqueueBuffers(music->source, 1, &buffer);
		}
		alDeleteSources(1, &music->source);
		music_map[music->dir].music.reset_buffers();
	}
	active_music.clear();
}

void SoundManager::pause_all_reserved_sounds() {
	for (SoundPlayer* player : sound_players) {
		player->pause_reserved_sound();
	}
}

void SoundManager::pause_all_sounds() {
	for (SoundPlayer* player : sound_players) {
		player->pause_sound_all();
	}
}

void SoundManager::resume_all_reserved_sounds() {
	for (SoundPlayer* player : sound_players) {
		player->resume_reserved_sound();
	}
}

void SoundManager::resume_all_sounds() {
	for (SoundPlayer* player : sound_players) {
		player->resume_sound_all();
	}
}

void SoundManager::register_sound_player(SoundPlayer* sound_player) {
	sound_players.push_front(sound_player);
}

void SoundManager::unregister_sound_player(SoundPlayer* sound_player) {
	sound_players.remove(sound_player);
}

void SoundManager::clear_sound_players() {
	sound_players.clear();
}

Sound& SoundManager::get_sound(std::string dir) {
	if (!sound_map.contains(dir)) {
		load_sound(dir);
	}
	sound_map[dir].user_count++;
	return sound_map[dir].sound;
}

Music& SoundManager::get_music(std::string dir) {
	if (!music_map.contains(dir)) {
		load_music(dir);
	}
	music_map[dir].user_count++;
	return music_map[dir].music;
}

void SoundManager::load_sound(std::string dir) {
	sound_map[dir].sound = Sound(dir);
	sound_map[dir].user_count = 0;
}

void SoundManager::load_music(std::string dir) {
	music_map[dir].music = Music(dir);
	music_map[dir].user_count = 0;
}

void SoundManager::unuse_sound(std::string dir) {
	if (sound_map.contains(dir)) {
		sound_map[dir].user_count--;
	}
}

void SoundManager::unuse_music(std::string dir) {
	if (music_map.contains(dir)) {
		music_map[dir].user_count--;
	}
}

void SoundManager::unload_sound(std::string dir, bool strict) {
	if (sound_map.contains(dir)) {
		if (sound_map[dir].user_count == 0 || !strict) {
			alDeleteBuffers(1, &sound_map[dir].sound.buffer);
			sound_map.erase(dir);
		}
	}
}

void SoundManager::unload_music(std::string dir, bool strict) {
	if (music_map.contains(dir)) {
		if (music_map[dir].user_count == 0 || !strict) {
			alDeleteBuffers(music_map[dir].music.num_buffers, &music_map[dir].music.buffers[0]);
			music_map.erase(dir);
		}
	}
}

void SoundManager::unload_all_sounds() {
	for (const auto& sound : sound_map) {
		alDeleteBuffers(1, &sound.second.sound.buffer);
	}
	sound_map.clear();
}

void SoundManager::unload_all_music() {
	for (const auto& music : music_map) {
		alDeleteBuffers(music.second.music.num_buffers, &music.second.music.buffers[0]);
	}
	music_map.clear();
}

void SoundManager::unload_unused() {
	for (const auto& sound : sound_map) {
		if (!sound.second.user_count) {
			alDeleteBuffers(1, &sound.second.sound.buffer);
			sound_map.erase(sound.first);
		}
	}
	for (const auto& music : music_map) {
		if (!music.second.user_count) {
			alDeleteBuffers(music.second.music.num_buffers, &music.second.music.buffers[0]);
			music_map.erase(music.first);
		}
	}
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
	Music& music = music_map[music_instance.dir].music;
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