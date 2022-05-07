#include "SoundManager.h"
#include "GameSettings.h"
#include "SDL/SDL_audio.h"
SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() {
	init();
}

SoundManager* SoundManager::get_instance() {
	if (instance == nullptr) {
		instance = new SoundManager;
	}
	return instance;
}

void SoundManager::init() {
	//VC
	add_sound_info("roy_attack_01", "roy", SOUND_KIND_VC, SOUND_TYPE_NORMAL);

	//Music
	add_sound_info("Atlas_Theme", "stage", SOUND_KIND_MUSIC, SOUND_TYPE_LOOP);
	add_sound_info("Vesuvius_Theme", "stage", SOUND_KIND_MUSIC, SOUND_TYPE_LOOP);
}

void SoundManager::play_sound(int object_id, int sound_kind, std::string name, int volume) {
	if (sound_name_map.find(name) == sound_name_map.end()) {
		std::cerr << "Sound " << name << " not loaded!\n";
		return;
	}
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid sound player!\n";
		return;
	}

	SDL_LockAudio();
	active_sounds[id2index[object_id]][sound_kind].push_back(loaded_sounds[sound_name_map[name]].instantiate(volume));
	SDL_UnlockAudio();
}

void SoundManager::pause_sound_all(int object_id, int sound_kind) {
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid sound player!\n";
		return;
	}
	std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin();
	SDL_LockAudio();
	for (int i = 0, max = active_sounds[id2index[object_id]][sound_kind].size(); i < max; i++) {
		it->active = false;
		std::advance(it, i + 1);
	}
	SDL_UnlockAudio();
}

void SoundManager::resume_sound_all(int object_id, int sound_kind) {
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid sound player!\n";
		return;
	}
	std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin();
	SDL_LockAudio();
	for (int i = 0, max = active_sounds[id2index[object_id]][sound_kind].size(); i < max; i++) {
		it->active = true;
		std::advance(it, i + 1);
	}
	SDL_UnlockAudio();
}

void SoundManager::stop_sound(int object_id, int sound_kind, std::string name) {
	if (sound_name_map.find(name) == sound_name_map.end()) {
		std::cerr << "Sound " << name << " not loaded!\n";
		return;
	}
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid sound player!\n";
		return;
	}
	std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin();
	for (int i = 0, max = active_sounds[id2index[object_id]][sound_kind].size(); i < max; i++) {
		if (it->sound->info->name == name) {
			SDL_LockAudio();
			active_sounds[id2index[object_id]][sound_kind].erase(it);
			SDL_UnlockAudio();
			return;
		}
		std::advance(it, i + 1);
	}
}

void SoundManager::stop_sound_all(int object_id, int sound_kind) {
	if (sound_kind != -1) {
		if (id2index.find(object_id) == id2index.end()) {
			std::cerr << "ID " << object_id << " is not a valid sound player!\n";
			return;
		}
		SDL_LockAudio();
		active_sounds[id2index[object_id]][sound_kind].clear();
		SDL_UnlockAudio();
	}
	else {
		SDL_LockAudio();
		for (int i = 0, max = active_sounds.size(); i < max; i++) {
			for (int i2 = 0; i2 < SOUND_KIND_MAX; i2++) {
				active_sounds[i][i2].clear();
			}
		}
		SDL_UnlockAudio();
	}
}

void SoundManager::load_sound(std::string name) {
	if (sound_info_map.find(name) == sound_info_map.end()) {
		std::cerr << "Sound " << name << " doesn't exist!\n";
		return;
	}
	if (sound_name_map.find(name) != sound_name_map.end()) {
		return;
	}

	SoundInfo *sound_info = &this->sound_info[sound_info_map[name]];

	Sound to_load(sound_info);
	sound_name_map[name] = loaded_sounds.size();

	SDL_AudioSpec wave;
	Uint8* data;
	unsigned int duration;
	SDL_AudioCVT cvt;
	const char* file = (sound_info->dir).c_str();

	if (SDL_LoadWAV(file, &wave, &data, &duration) == NULL) {
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return;
	}
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_F32SYS, 2, 22050);

	cvt.len = duration;
	cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult);
	SDL_memcpy(cvt.buf, data, duration);

	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);

	to_load.data = cvt.buf;
	to_load.duration = cvt.len_cvt;

	if (sound_info->sound_type == SOUND_TYPE_LOOP) {
		SDL_AudioSpec loop_wave;
		Uint8* loop_data;
		unsigned int loop_duration;
		SDL_AudioCVT loop_cvt;
		const char* loop_file = (sound_info->loop_dir).c_str();

		if (SDL_LoadWAV(loop_file, &loop_wave, &loop_data, &loop_duration) == NULL) {
			fprintf(stderr, "Couldn't load %s: %s\n", loop_file, SDL_GetError());
			return;
		}
		SDL_BuildAudioCVT(&loop_cvt, loop_wave.format, loop_wave.channels, loop_wave.freq, AUDIO_F32SYS, 2, 22050);

		loop_cvt.len = loop_duration;
		loop_cvt.buf = (Uint8*)SDL_malloc(loop_cvt.len * loop_cvt.len_mult);
		SDL_memcpy(loop_cvt.buf, loop_data, loop_duration);

		SDL_ConvertAudio(&loop_cvt);
		SDL_FreeWAV(loop_data);

		SDL_LockAudio();
		to_load.loop_data = loop_cvt.buf;
		to_load.loop_duration = loop_cvt.len_cvt;
		SDL_UnlockAudio();
	}

	loaded_sounds.push_back(to_load);
}

void SoundManager::unload_sound(std::string name) {
	if (sound_name_map.find(name) == sound_name_map.end()) {
		std::cout << "Sound " << name << " is not loaded!\n";
		return;
	}
	Sound& to_unload = loaded_sounds[sound_name_map[name]];
	if (to_unload.data != nullptr) {
		SDL_free(to_unload.data);
		to_unload.data = nullptr;
	}
	if (to_unload.loop_data != nullptr) {
		SDL_free(to_unload.loop_data);
		to_unload.loop_data = nullptr;
	}
	sound_name_map.erase(name);
	//Note: This will leave an empty slot in the list of loaded sounds. Technically leaks memory
	//but I know this function will rarely actually get called, and either way calling unload_all_sounds
	//will do a proper clear
}

void SoundManager::unload_all_sounds() {
	SDL_LockAudio();
	for (int i = 0, max = loaded_sounds.size(); i < max; i++) {
		if (loaded_sounds[i].data != nullptr) {
			SDL_free(loaded_sounds[i].data);
			loaded_sounds[i].data = nullptr;
		}
		if (loaded_sounds[i].loop_data != nullptr) {
			SDL_free(loaded_sounds[i].loop_data);
			loaded_sounds[i].loop_data = nullptr;
		}
	}
	SDL_UnlockAudio();
	loaded_sounds.clear();
	sound_name_map.clear();
}

void SoundManager::add_sound_player(int object_id) {
	id2index[object_id] = active_sounds.size();
	active_sounds.resize(active_sounds.size() + 1);
}

void SoundManager::remove_sound_players() {
	active_sounds.clear();
	id2index.clear();
}

void SoundManager::add_sound_info(std::string name, std::string dir, int sound_kind, int sound_type, int volume) {
	sound_info_map[name] = sound_info.size();
	SoundInfo to_add(name, dir, sound_kind, sound_type, volume);
	sound_info.push_back(to_add);
}

void audio_callback(void* unused, Uint8* stream, int len) {
	unsigned int diff; //How much leftover space we have if the length of the stream > the length of the track
	Uint8* source; //Audio data that will be filled by a given track
	Uint8* data; //Will either be the regular track or the loop track; both are stored in the same sound instance if the latter exists
	unsigned int dlen; //Ditto for length
	unsigned int mlen; //Maximum length
	int vol; //Volume value to be multiplied by the values in the user's settings
	SoundManager* sound_manager = SoundManager::get_instance();
	
	SDL_memset(stream, 0, len); //Clear the stream

	for (int i = 0, max = sound_manager->active_sounds.size(); i < max; i++) {
		for (int i2 = 0; i2 < SOUND_KIND_MAX; i2++) {
			std::list<SoundInstance>::iterator it = sound_manager->active_sounds[i][i2].begin();
			for (int i3 = 0, max3 = sound_manager->active_sounds[i][i2].size(); i3 < max3; i3++) {
				if (it->active) {
					SoundInstance& sound_instance = *it;
					diff = 0;
					data = sound_instance.sound->data;
					dlen = sound_instance.sound->duration;
					if (sound_instance.sound->info->sound_kind == SOUND_KIND_SE) {
						vol = getGameSetting("se_vol") * get_relative_one_percent(sound_instance.volume, 128);
					}
					else if (sound_instance.sound->info->sound_kind == SOUND_KIND_VC) {
						vol = getGameSetting("vc_vol") * get_relative_one_percent(sound_instance.volume, 128);
					}
					else {
						vol = getGameSetting("music_vol") * get_relative_one_percent(sound_instance.volume, 128);
					}

					if (sound_instance.sound->info->sound_type == SOUND_TYPE_LOOP && sound_instance.looped) {
						data = sound_instance.sound->loop_data;
						dlen = sound_instance.sound->loop_duration;
					}

					source = (Uint8*)SDL_malloc(len); //Allocate enough memory to hold our sound data
					if (sound_instance.pos + len > dlen) { //Check if we're about to hit the end of the file, and if so by how much
						diff = (sound_instance.pos + len) - dlen;
					}

					mlen = clamp(0, len, dlen - sound_instance.pos);

					//Copy as much data from the audio track as we have into the source variable, making sure not to copy data that doesn't exist.

					SDL_memcpy(source, &data[sound_instance.pos], mlen);

					sound_instance.pos += len; //Add the length of the stream to the audio's position.

					if (diff != 0) { //If we went over
						if (sound_instance.sound->info->sound_type == SOUND_TYPE_LOOP) {
							if (!sound_instance.looped) { //If we've never looped before, clarify that we're getting the rest of our
								//data from the looped version, and that we are now in the loop state.
								sound_instance.looped = true;
								data = sound_instance.sound->loop_data;
							}
							sound_instance.pos = 0;
							SDL_memcpy(&source[len - diff], &data[sound_instance.pos], diff);
							sound_instance.pos += diff;

						}
						else { //Otherwise just get that shit outta here
							sound_manager->active_sounds[i][i2].erase(it);
						}
					}
					SDL_MixAudio(stream, source, mlen, vol);
					SDL_free(source);
				}
			}
		}
	}
}
