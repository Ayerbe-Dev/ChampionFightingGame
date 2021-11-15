#include "SoundManager.h"
SoundInfo sounds[3][MAX_SOUNDS];
extern bool debug;
extern bool can_play_non_music;

SoundManager::SoundManager() {}

SoundManager::SoundManager(bool init) {
	if (init) {
		//you had to be there
		cout << "  /$$$$$$                        /$$               /$$                                  /$$$$$$                        /$$                               /$$$$$$           /$$   /$$    " << endl;
		cout << " /$$__  $$                      | $$              | $$                                 /$$__  $$                      | $$                              |_  $$_/          |__/  | $$    " << endl;
		cout << "| $$  \\__/  /$$$$$$  /$$$$$$$  /$$$$$$    /$$$$$$$| $$$$$$$   /$$$$$$   /$$$$$$       | $$  \\__/  /$$$$$$  /$$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$         | $$   /$$$$$$$  /$$ /$$$$$$  " << endl;
		cout << "| $$ /$$$$ |____  $$| $$__  $$|_  $$_/   /$$_____/| $$__  $$ /$$__  $$ /$$__  $$      | $$       /$$__  $$| $$__  $$|_  $$_/   /$$__  $$ /$$__  $$        | $$  | $$__  $$| $$|_  $$_/  " << endl;
		cout << "| $$|_  $$  /$$$$$$$| $$  \\ $$  | $$    | $$      | $$  \\ $$| $$$$$$$$| $$  \\__/      | $$      | $$$$$$$$| $$  \\ $$  | $$    | $$$$$$$$| $$  \\__/        | $$  | $$  \\ $$| $$  | $$    " << endl;
		cout << "| $$  \\ $$ /$$__  $$| $$  | $$  | $$ /$$| $$      | $$  | $$| $$_____/| $$            | $$    $$| $$_____/| $$  | $$  | $$ /$$| $$_____/| $$              | $$  | $$  | $$| $$  | $$ /$$" << endl;
		cout << "|  $$$$$$/|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$$| $$  | $$|  $$$$$$$| $$            |  $$$$$$/|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$$| $$             /$$$$$$| $$  | $$| $$  |  $$$$/" << endl;
		cout << " \\______/  \\_______/|__/  |__/   \\___/   \\_______/|__/  |__/ \\_______/|__/             \\______/  \\_______/|__/  |__/   \\___/   \_______/|__/            |______/|__/  |__/|__/   \\___/  " << endl << endl;

	}
	hyperInit();
}

void SoundManager::hyperInit() {
	roy_vc[ROY_VC_ATTACK_01] = Sound("attack_01", SOUND_KIND_VC, CHARA_KIND_ROY);

	music[MUSIC_KIND_ATLAS_STAGE] = Sound("Atlas_Theme", SOUND_KIND_MUSIC, 0, 128, SOUND_TYPE_LOOP);
}

int SoundManager::playCommonSE(int se, int id) {
	Sound sound = common_se[se];
	if (sound.name == "") {
		return -1;
	}
	playSound(sound, id);
	return 0;
}

int SoundManager::playCharaSE(int se, int id) {
	Sound sound;
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY): {
			sound = roy_se[se];
		} break;
		case(CHARA_KIND_ERIC):{
			sound = eric_se[se];
		} break;
		case(CHARA_KIND_ATLAS): {
			sound = atlas_se[se];
		} break;
		default: {} break;
	}
	if (sound.name == "") {
		return -1;
	}
	playSound(sound, id);
	return 0;
}

int SoundManager::playVC(int vc, int id) {
	Sound sound;
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			sound = roy_vc[vc];
		} break;
		case(CHARA_KIND_ERIC):
		{
			sound = eric_vc[vc];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			sound = atlas_vc[vc];
		} break;
		default: {} break;
	}
	if (sound.name == "") {
		return -1;
	}
	playSound(sound, id);
	return 0;
}

int SoundManager::playMusic(int music_kind) {
	Sound sound = music[music_kind];
	if (sound.name == "") {
		return -1;
	}
	playSound(sound, 2);
	return 0;
}

void SoundManager::playSound(Sound sound, int id) {
	const char* dir = (sound.dir).c_str();
	addSoundToIndex(sound, id);
}

int SoundManager::findSoundIndex(Sound sound, int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.name == sound.dir) {
			cout << i << endl;
			return i;
		}
	}
	return MAX_SOUNDS;
}

void SoundManager::endCommonSE(int se, int id) {
	Sound sound = common_se[se];
	endSound(sound, id);
}

void SoundManager::endCharaSE(int se, int id) {
	Sound sound;
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			sound = roy_se[se];
		} break;
		case(CHARA_KIND_ERIC):
		{
			sound = eric_se[se];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			sound = atlas_se[se];
		} break;
		default: {} break;
	}
	endSound(sound, id);
}

void SoundManager::endVC(int vc, int id) {
	Sound sound;
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			sound = roy_vc[vc];
		} break;
		case(CHARA_KIND_ERIC):
		{
			sound = eric_vc[vc];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			sound = atlas_vc[vc];
		} break;
		default: {} break;
	}
	endSound(sound, id);
}

void SoundManager::endSEAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_SE) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
		}
	}
}

void SoundManager::endVCAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_VC) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
		}
	}
}

void SoundManager::endMusic(int music_kind) {
	Sound sound = music[music_kind];
	endSound(sound, 2);
}

void SoundManager::endSound(Sound sound, int id) {
	int clear_index = findSoundIndex(sound, id);
	sounds[id][clear_index].dpos = 0;
	sounds[id][clear_index].dlen = 0;
	sounds[id][clear_index].loop_dlen = 0;
	SDL_free(sounds[id][clear_index].data);
	sounds[id][clear_index].data = NULL;
	sounds[id][clear_index].loop_data = NULL;
}

void SoundManager::endSoundAll() {
	SDL_LockAudio();
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (sounds[i][i2].data) {
				SDL_free(sounds[i][i2].data);

				sounds[i][i2].data = NULL;
				sounds[i][i2].loop_data = NULL;
				sounds[i][i2].dpos = 0;
				sounds[i][i2].dlen = 0;
				sounds[i][i2].loop_dlen = 0;
			}
		}
	}
	SDL_UnlockAudio();
}

Sound::Sound() {
	sound_kind = SOUND_KIND_MAX;
}

Sound::Sound(string name, int sound_kind, int chara_kind, int volume, int sound_type) {
	this->name = name;
	this->sound_kind = sound_kind;
	this->sound_type = sound_type;
	this->volume = volume;
	looped = false;
	switch (sound_kind) {
		case (SOUND_KIND_MUSIC):
		{
			this->dir = "resource/sound/bgm/" + name + ".wav";
			this->loop_dir = "resource/sound/bgm/" + name + "_loop.wav";
		} break;
		case (SOUND_KIND_SE): {
			switch (chara_kind) {
				default: {
					this->dir = "resource/sound/se/common/" + name + ".wav";
				} break;
				case (CHARA_KIND_ROY): {
					this->dir = "resource/sound/se/roy/" + name + ".wav";
				} break;
				case (CHARA_KIND_ERIC):	{
					this->dir = "resource/sound/se/eric/" + name + ".wav";
				} break;
				case (CHARA_KIND_ATLAS): {
					this->dir = "resource/sound/se/atlas/" + name + ".wav";
				} break;
			}
		} break;
		case (SOUND_KIND_VC): {
			switch (chara_kind) {
				default: {} break;
				case (CHARA_KIND_ROY):
				{
					this->dir = "resource/sound/vc/roy/" + name + ".wav";
				} break;
				case (CHARA_KIND_ERIC):
				{
					this->dir = "resource/sound/vc/eric/" + name + ".wav";
				} break;
				case (CHARA_KIND_ATLAS):
				{
					this->dir = "resource/sound/vc/atlas/" + name + ".wav";
				} break;

			}
		} break;
	}
}

void addSoundToIndex(Sound sound, int id) {
	int index;

	SDL_AudioSpec wave;
	u8* data;
	u32 dlen;
	SDL_AudioCVT cvt;
	const char* file = (sound.dir).c_str();

	SDL_AudioSpec loop_wave;
	u8* loop_data;
	u32 loop_dlen;
	SDL_AudioCVT loop_cvt;
	const char* loop_file = (sound.loop_dir).c_str();

	for (index = 0; index < MAX_SOUNDS; index++) {
		if (!sounds[id][index].data) {
			break;
		}
	}
	if (index == MAX_SOUNDS) {
		return;
	}
	if (SDL_LoadWAV(file, &wave, &data, &dlen) == NULL) {
		fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return;
	}
	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_F32SYS, 2, 22050);

	cvt.len = dlen;
	cvt.buf = (Uint8*)SDL_malloc(cvt.len * cvt.len_mult); //Converting the audio goes through multiple passes, some of which increase the size, so
	//we allocate enough memory to handle the size during the largest pass
	SDL_memcpy(cvt.buf, data, dlen);

	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);

	SDL_LockAudio();
	sounds[id][index].data = cvt.buf;
	sounds[id][index].dlen = cvt.len_cvt;
	sounds[id][index].dpos = 0;
	SDL_UnlockAudio();

	if (sound.sound_type == SOUND_TYPE_LOOP) { //If the sound effect is also designed to loop, we load that data as well
		SDL_LockAudio(); //If we're loading looping music, it's not midgame, so this is fine
		if (SDL_LoadWAV(loop_file, &loop_wave, &loop_data, &loop_dlen) == NULL) {
			fprintf(stderr, "Couldn't load %s: %s\n", loop_file, SDL_GetError());
			return;
		}
		SDL_BuildAudioCVT(&loop_cvt, loop_wave.format, loop_wave.channels, loop_wave.freq, AUDIO_F32SYS, 2, 22050);

		loop_cvt.len = loop_dlen;
		loop_cvt.buf = (Uint8*)SDL_malloc(loop_cvt.len * loop_cvt.len_mult); 
		SDL_memcpy(loop_cvt.buf, loop_data, loop_dlen);

		SDL_ConvertAudio(&loop_cvt);
		SDL_FreeWAV(loop_data);

		sounds[id][index].loop_data = loop_cvt.buf;
		sounds[id][index].loop_dlen = loop_cvt.len_cvt;
		sounds[id][index].dpos = 0;
		SDL_UnlockAudio();
	}
	sounds[id][index].sound = sound; //We don't put this in until we finish, to prevent a song with a loop to play the first ~3 ticks twice
}

void audio_callback(void* unused, Uint8* stream, int len) {

	u32 diff = 0; //How much leftover space we have if the length of the stream > the length of the track
	u8* source; //Audio data that will be filled by a given track
	u8* data; //Will either be the regular track or the loop track; both are stored in the same sound instance if the latter exists
	u32 dlen; //Ditto for length

	SDL_memset(stream, 0, len); //Clear the stream

	for (int i = 0; i < MAX_SOUNDS; i++) {
		for (int i2 = 0; i2 < 3; i2++) {
			if (sounds[i2][i].data) { //Tbh we should probably be preloading EVERYTHING and just having an "active" field in the Sound class, but 
				//for now we can just check to see if the slot is empty
				if (can_play_non_music || ((sounds[i2][i].sound.sound_kind == SOUND_KIND_MUSIC))) { //Can Play Non Music is just "Are we in debug mode"
					data = sounds[i2][i].data;
					dlen = sounds[i2][i].dlen;
					if (sounds[i2][i].sound.sound_type == SOUND_TYPE_LOOP && sounds[i2][i].sound.looped) { 
						data = sounds[i2][i].loop_data;
						dlen = sounds[i2][i].loop_dlen;
					}

					source = (u8*)SDL_malloc(len); //Allocate enough memory to hold our sound data
					if (sounds[i2][i].dpos + len > dlen) { //Check if we're about to hit the end of the file, and if so by how much
						diff = (sounds[i2][i].dpos + len) - dlen;
					}

					//Copy as much data from the audio track as we have into the source variable, making sure not to copy data that doesn't exist.

					SDL_memcpy(source, &data[sounds[i2][i].dpos], clamp(len, len, dlen));
	
					sounds[i2][i].dpos += len; //Add the length of the stream to the audio's position.

					if (diff != 0) { //If we went over
						if (sounds[i2][i].sound.sound_type == SOUND_TYPE_LOOP) { 
							if (!sounds[i2][i].sound.looped) { //If we've never looped before, clarify that we're getting the rest of our
								//data from the looped version, and that we are now in the loop state.
								sounds[i2][i].sound.looped = true;
								data = sounds[i2][i].loop_data;
							}
							sounds[i2][i].dpos = 0;
							SDL_memcpy(&source[len - diff], &data[sounds[i2][i].dpos], diff);
							sounds[i2][i].dpos += diff;

						}
						else { //Otherwise just get that shit outta here
							sounds[i2][i].data = NULL;
							sounds[i2][i].dpos = 0;
							sounds[i2][i].dlen = 0;
						}
					}

					SDL_MixAudio(stream, source, len, sounds[i2][i].sound.volume);
					SDL_free(source);
				}
			}
		}
	}
}
