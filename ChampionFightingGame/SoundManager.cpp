#include "SoundManager.h"
#include "SaveManager.h"
#include "SDL/SDL_audio.h"
#include "utils.h"

SoundManager::SoundManager() {
	SDL_AudioSpec format;
	format.freq = 22050;
	format.format = AUDIO_F32SYS;
	format.channels = 2;
	format.samples = 361;
	/*
	The number of samples needs to be a perfect square, and that value represents how much data will go into each iteration of the callback. Since the
	callback only runs when the program determines it is time to load in more audio data, this value also effectively sets how often the callback will
	run. The value of 361 (19*19) causes the callback to run ~61 times per second, which allows it to sync up with frame advance.
	*/
	format.callback = audio_callback;
	format.userdata = NULL;

	if (SDL_OpenAudio(&format, NULL) < 0) {
		printf("Error opening SDL_audio: %s\n", SDL_GetError());
	}
	SDL_PauseAudio(0);

	populate_sounds();
}

void SoundManager::populate_sounds() {
	//VC
	add_sound_info("rowan_attack_01", "rowan", SOUND_KIND_VC, SOUND_TYPE_NORMAL);

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
	if (object_id != -1) {
		if (id2index.find(object_id) == id2index.end()) {
			std::cerr << "ID " << object_id << " is not a valid sound player!\n";
			return;
		}
		if (sound_kind != -1) {
			SDL_LockAudio();
			for (std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin(), max = active_sounds[id2index[object_id]][sound_kind].end(); it != max; it++) {
				it->active = false;
			}
			SDL_UnlockAudio();
		}
		else {
			SDL_LockAudio();
			for (int i = 0; i < SOUND_KIND_MAX; i++) {
				for (std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][i].begin(), max = active_sounds[id2index[object_id]][i].end(); it != max; it++) {
					it->active = false;
				}
			}
			SDL_UnlockAudio();
		}
	}
	else {
		SDL_LockAudio();
		for (int i = 0, max = active_sounds.size(); i < max; i++) {
			for (int i2 = 0; i2 < SOUND_KIND_MAX; i2++) {
				for (std::list<SoundInstance>::iterator it = active_sounds[i][i2].begin(), max = active_sounds[i][i2].end(); it != max; it++) {
					it->active = false;
				}
			}
		}
		SDL_UnlockAudio();
	}
}

void SoundManager::resume_sound_all(int object_id, int sound_kind) {
	if (object_id != -1) {
		if (id2index.find(object_id) == id2index.end()) {
			std::cerr << "ID " << object_id << " is not a valid sound player!\n";
			return;
		}
		if (sound_kind != -1) {
			SDL_LockAudio();
			for (std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin(), max = active_sounds[id2index[object_id]][sound_kind].end(); it != max; it++) {
				it->active = true;
			}
			SDL_UnlockAudio();
		}
		else {
			SDL_LockAudio();
			for (int i = 0; i < SOUND_KIND_MAX; i++) {
				for (std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][i].begin(), max = active_sounds[id2index[object_id]][i].end(); it != max; it++) {
					it->active = true;
				}
			}
			SDL_UnlockAudio();
		}
	}
	else {
		SDL_LockAudio();
		for (int i = 0, max = active_sounds.size(); i < max; i++) {
			for (int i2 = 0; i2 < SOUND_KIND_MAX; i2++) {
				for (std::list<SoundInstance>::iterator it = active_sounds[i][i2].begin(), max = active_sounds[i][i2].end(); it != max; it++) {
					it->active = true;
				}
			}
		}
		SDL_UnlockAudio();
	}
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
	for (std::list<SoundInstance>::iterator it = active_sounds[id2index[object_id]][sound_kind].begin(), max = active_sounds[id2index[object_id]][sound_kind].end(); it != max; it++) {
		if (it->sound->info->name == name) {
			SDL_LockAudio();
			active_sounds[id2index[object_id]][sound_kind].erase(it);
			SDL_UnlockAudio();
			return;
		}
	}
}

void SoundManager::stop_sound_all(int object_id, int sound_kind) {
	if (object_id != -1) {
		if (id2index.find(object_id) == id2index.end()) {
			std::cerr << "ID " << object_id << " is not a valid sound player!\n";
			return;
		}
		if (sound_kind != -1) {
			SDL_LockAudio();
			active_sounds[id2index[object_id]][sound_kind].clear();
			SDL_UnlockAudio();
		}
		else {
			SDL_LockAudio();
			for (int i = 0; i < SOUND_KIND_MAX; i++) {
				active_sounds[id2index[object_id]][i].clear();
			}
			SDL_UnlockAudio();
		}
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

int SoundManager::add_sound_player() {
	int i;
	for (i = 50; id2index.contains(i); i++); //Slots 0-29 are reserved for fighters and projectiles, slots
	//30-49 are reserved for stage assets. 50+ is where we get to things that aren't necessarily assigned
	//their own IDs
	id2index[i] = active_sounds.size();
	active_sounds.resize(active_sounds.size() + 1);
}

void SoundManager::add_sound_player(int object_id) {
	id2index[object_id] = active_sounds.size();
	active_sounds.resize(active_sounds.size() + 1);
}

void SoundManager::remove_sound_player(int id) {
	stop_sound_all(id);
	active_sounds.erase(active_sounds.begin() + id2index[id]);
	id2index.erase(id);
}

void SoundManager::remove_sound_players() {
	stop_sound_all();
	active_sounds.clear();
	id2index.clear();
}

void SoundManager::add_sound_info(std::string name, std::string dir, int sound_kind, int sound_type, int volume) {
	sound_info_map[name] = sound_info.size();
	SoundInfo to_add(name, dir, sound_kind, sound_type, volume);
	sound_info.push_back(to_add);
}

SoundManager* SoundManager::instance = nullptr;
SoundManager* SoundManager::get_instance() {
	if (instance == nullptr) {
		instance = new SoundManager;
	}
	return instance;
}

void SoundManager::destroy_instance() {
	unload_all_sounds();
	if (instance != nullptr) {
		delete instance;
	}
}

void audio_callback(void* unused, Uint8* stream, int len) {
	unsigned int diff; //How much leftover space we have if the length of the stream > the length of the track
	Uint8* source; //Audio data that will be filled by a given track
	Uint8* data; //Will either be the regular track or the loop track; both are stored in the same sound instance if the latter exists
	unsigned int dlen; //Ditto for length
	unsigned int mlen; //Maximum length
	int vol; //Volume value to be multiplied by the values in the user's settings
	SoundManager* sound_manager = SoundManager::get_instance();
	SaveManager* save_manager = SaveManager::get_instance();

	
	SDL_memset(stream, 0, len); //Clear the stream

	for (int i = 0, max = sound_manager->active_sounds.size(); i < max; i++) {
		for (int i2 = 0; i2 < SOUND_KIND_MAX; i2++) {
			for (std::list<SoundInstance>::iterator it = sound_manager->active_sounds[i][i2].begin(); it != sound_manager->active_sounds[i][i2].end(); it++) {
				if (it->active) {
					diff = 0;
					data = it->sound->data;
					dlen = it->sound->duration;
					if (it->sound->info->sound_kind == SOUND_KIND_SE) {
						vol = save_manager->get_game_setting("se_vol") * get_relative_one_percent(it->volume, 128);
					}
					else if (it->sound->info->sound_kind == SOUND_KIND_VC) {
						vol = save_manager->get_game_setting("vc_vol") * get_relative_one_percent(it->volume, 128);
					}
					else {
						vol = save_manager->get_game_setting("music_vol") * get_relative_one_percent(it->volume, 128);
					}

					if (it->sound->info->sound_type == SOUND_TYPE_LOOP && it->looped) {
						data = it->sound->loop_data;
						dlen = it->sound->loop_duration;
					}

					source = (Uint8*)SDL_malloc(len); //Allocate enough memory to hold our sound data
					if (it->pos + len > dlen) { //Check if we're about to hit the end of the file, and if so by how much
						diff = (it->pos + len) - dlen;
					}

					mlen = clamp(0, len, dlen - it->pos);

					//Copy as much data from the audio track as we have into the source variable, making sure not to copy data that doesn't exist.

					SDL_memcpy(source, &data[it->pos], mlen);

					it->pos += len; //Add the length of the stream to the audio's position.

					if (diff != 0) { //If we went over
						if (it->sound->info->sound_type == SOUND_TYPE_LOOP) {
							if (!it->looped) { //If we've never looped before, clarify that we're getting the rest of our
								//data from the looped version, and that we are now in the loop state.
								it->looped = true;
								data = it->sound->loop_data;
							}
							it->pos = 0;
							SDL_memcpy(&source[len - diff], &data[it->pos], diff);
							it->pos += diff;

						}
						else { //Otherwise just get that shit outta here
							if (sound_manager->active_sounds[i][i2].size() != 1) {
								it = sound_manager->active_sounds[i][i2].erase(it);
							}
							else {
								sound_manager->active_sounds[i][i2].erase(it);
								break;
							}
						}
					}
					SDL_MixAudio(stream, source, mlen, vol);
					SDL_free(source);
				}
			}
		}
	}
}
