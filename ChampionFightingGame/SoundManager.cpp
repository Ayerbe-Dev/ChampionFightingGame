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

	stage_music[STAGE_MUSIC_ATLAS] = Sound("Atlas_Theme", SOUND_KIND_STAGE_MUSIC, 0, SOUND_TYPE_LOOP, 2050480);
//2062032
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

int SoundManager::playStageMusic(int stage_kind) {
	Sound sound = stage_music[stage_kind];
	if (sound.name == "") {
		return -1;
	}
	playSound(sound, 2);
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
	int index = 0;
	const char* dir = (sound.dir).c_str();
	addSoundToIndex(sound, &index, id);
	if (index != MAX_SOUNDS) {
		active_sounds[id][index] = sound;
	}
}

int SoundManager::findSoundIndex(Sound sound, int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (active_sounds[id][i].name == sound.dir) {
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
		if (active_sounds[id][i].sound_kind == SOUND_KIND_SE) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
		}
	}
}

void SoundManager::endVCAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (active_sounds[id][i].sound_kind == SOUND_KIND_VC) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
		}
	}
}

void SoundManager::endStageMusic(int stage_kind) {
	Sound sound = stage_music[stage_kind];
	endSound(sound, 2);
}

void SoundManager::endMusic(int music_kind) {
	Sound sound = music[music_kind];
	endSound(sound, 2);
}

void SoundManager::endSound(Sound sound, int id) {
	int clear_index = findSoundIndex(sound, id);
	sounds[id][clear_index].dpos = 0;
	sounds[id][clear_index].dlen = 0;
	SDL_free(sounds[id][clear_index].data);
	sounds[id][clear_index].data = NULL;
}

void SoundManager::endSoundAll() {
	SDL_LockAudio();
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (sounds[i][i2].data) {
				SDL_free(sounds[i][i2].data);

				sounds[i][i2].data = NULL;
				sounds[i][i2].dpos = 0;
				sounds[i][i2].dlen = 0;
			}
		}
	}
	SDL_UnlockAudio();
}

void SoundManager::checkSoundEnd() {
	SDL_LockAudio();
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (sounds[i][i2].data) {
				if (sounds[i][i2].dpos >= sounds[i][i2].dlen) {
					if (active_sounds[i][i2].sound_type == SOUND_TYPE_LOOP) {
						sounds[i][i2].dpos = active_sounds[i][i2].loop_point;
					}
					else {
						if (sounds[i][i2].data) {
							SDL_free(sounds[i][i2].data);

							sounds[i][i2].data = NULL;
							sounds[i][i2].dpos = 0;
							sounds[i][i2].dlen = 0;
						}
					}
				}
			}
		}
	}
	SDL_UnlockAudio();
}

Sound::Sound() {
	sound_kind = SOUND_KIND_MAX;
}

Sound::Sound(string name, int sound_kind, int chara_kind, int sound_type, int loop_point) {
	this->name = name;
	this->sound_kind = sound_kind;
	this->sound_type = sound_type;
	this->loop_point = loop_point;
	switch (sound_kind) {
		case (SOUND_KIND_MUSIC):
		{
			this->dir = "resource/sound/bgm/common/" + name + ".wav";
		} break;
		case (SOUND_KIND_STAGE_MUSIC):
		{
			this->dir = "resource/sound/bgm/stage/" + name + ".wav";
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

void audio_callback(void* unused, Uint8* stream, int len) {

	int i;
	u32 diff = 0;
	u8* source;
	SDL_memset(stream, 0, len);
	int unfreeze_music = 1; //Debugging. 0 = Music is frozen during debug mode, 1 = Normal, 2 = Print Music position, 3 = Music sped up 8x during debug mode

	for (i = 0; i < MAX_SOUNDS; i++) {
		for (int i2 = 0; i2 < 3; i2++) {
			if (sounds[i2][i].data) {
				if (can_play_non_music || 
					((sounds[i2][i].sound.sound_kind == SOUND_KIND_MUSIC || sounds[i2][i].sound.sound_kind == SOUND_KIND_STAGE_MUSIC)
					&& unfreeze_music)) {
					if (unfreeze_music == 2) {
						cout << "Music Pos: " << sounds[i2][i].dpos << endl;
					}

					source = (u8*)SDL_malloc(len); //Allocate the length we're gonna use for the sound
					if (sounds[i2][i].dpos + len > sounds[i2][i].dlen) {
						diff = (sounds[i2][i].dpos + len) - sounds[i2][i].dlen;
					}
					SDL_memcpy(source, &sounds[i2][i].data[sounds[i2][i].dpos], len);
					sounds[i2][i].dpos += len;
					if (debug && unfreeze_music == 3) {
						sounds[i2][i].dpos += len * 7;
					}
					if (diff != 0) {
						sounds[i2][i].dpos = sounds[i2][i].sound.loop_point;
						sounds[i2][i].dpos += diff;
						SDL_memcpy(&source[len - diff], &sounds[i2][i].data[sounds[i2][i].dpos], diff);
					}
					SDL_MixAudio(stream, source, len, SDL_MIX_MAXVOLUME);
					SDL_free(source);
				}
			}
		}
	}
}

void addSoundToIndex(Sound sound, int* ret, int id) {
	int index;
	SDL_AudioSpec wave;
	Uint8* data;
	Uint32 dlen;
	SDL_AudioCVT cvt;
	const char* file = (sound.dir).c_str();

	for (index = 0; index < MAX_SOUNDS; index++) {
		if (!sounds[id][index].data) {
			break;
		}
	}
	*ret = index;
	if (index == MAX_SOUNDS) {
		return;
	}

	if (SDL_LoadWAV(file, &wave, &data, &dlen) == NULL) { //Load the WAV
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
	sounds[id][index].sound = sound;
	SDL_UnlockAudio();
}