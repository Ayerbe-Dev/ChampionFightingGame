#include "SoundManager.h"
#include "GameSettings.h"
#include "SDL/SDL_audio.h"
SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager() {
	hyperInit();
}

void SoundManager::hyperInit() {
	roy_vc[ROY_VC_ATTACK_01] = SoundInfo("attack_01", SOUND_KIND_VC, CHARA_KIND_ROY);

	music[MUSIC_KIND_TRAINING_STAGE] = SoundInfo("Vesuvius_Theme", SOUND_KIND_MUSIC, 0, 128, SOUND_TYPE_LOOP);
	music[MUSIC_KIND_ATLAS_STAGE] = SoundInfo("Atlas_Theme", SOUND_KIND_MUSIC, 0, 128, SOUND_TYPE_LOOP);
}

void SoundManager::playCommonSE(int se, int id) {
	SoundInfo sound = common_se[se];
	playSound(sound, id);
}

void SoundManager::playCharaSE(int se, int id) {
	SoundInfo sound = getCharaSound(se, id, true);
	playSound(sound, id);
}

void SoundManager::playVC(int vc, int id) {
	SoundInfo sound = getCharaSound(vc, id, false);
	playSound(sound, id);
}

void SoundManager::playMusic(int music_kind) {
	SoundInfo sound = music[music_kind];
	playSound(sound, 2);
}

void SoundManager::playSound(SoundInfo sound, int id) {
	int index = findSoundIndex(sound, id);
	sounds[id][index].sound.active = true;
}

void SoundManager::pauseCommonSE(int se, int id) {
	SoundInfo sound = common_se[se];
	pauseSound(sound, id);
}

void SoundManager::pauseCharaSE(int se, int id) {
	SoundInfo sound = getCharaSound(se, id, true);
	pauseSound(sound, id);
}

void SoundManager::pauseVC(int vc, int id) {
	SoundInfo sound = getCharaSound(vc, id, false);
	pauseSound(sound, id);
}

void SoundManager::pauseSEAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_SE) {
			sounds[id][i].sound.active = false;
		}
	}
}

void SoundManager::pauseVCAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_VC) {
			sounds[id][i].sound.active = false;
		}
	}
}

void SoundManager::pauseMusic(int music_kind) {
	SoundInfo sound = music[music_kind];
	pauseSound(sound, 2);
}

void SoundManager::pauseSound(SoundInfo sound, int id) {
	int index = findSoundIndex(sound, id);
	sounds[id][index].sound.active = false;
}

void SoundManager::pauseSoundAll() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			sounds[i][i2].sound.active = false;
		}
	}
}

void SoundManager::resumeSEAll(int id) {
	for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
		if (!sounds[id][i2].sound.active && sounds[id][i2].dpos != 0 && sounds[id][i2].sound.sound_kind == SOUND_KIND_SE) {
			sounds[id][i2].sound.active = true;
		}
	}
}

void SoundManager::resumeVCAll(int id) {
	for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
		if (!sounds[id][i2].sound.active && sounds[id][i2].dpos != 0 && sounds[id][i2].sound.sound_kind == SOUND_KIND_VC) {
			sounds[id][i2].sound.active = true;
		}
	}
}

void SoundManager::resumeMusicAll() {
	for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
		if (!sounds[2][i2].sound.active && sounds[2][i2].dpos != 0 && sounds[2][i2].sound.sound_kind == SOUND_KIND_MUSIC) {
			sounds[2][i2].sound.active = true;
		}
	}
}

void SoundManager::resumeSoundAll(int id) {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (!sounds[i][i2].sound.active && sounds[i][i2].dpos != 0) {
				sounds[i][i2].sound.active = true;
			}
		}
	}
}

void SoundManager::stopCommonSE(int se, int id) {
	SoundInfo sound = common_se[se];
	stopSound(sound, id);
}

void SoundManager::stopCharaSE(int se, int id) {
	SoundInfo sound = getCharaSound(se, id, true);
	stopSound(sound, id);
}

void SoundManager::stopVC(int vc, int id) {
	SoundInfo sound = getCharaSound(vc, id, false);
	stopSound(sound, id);
}

void SoundManager::stopSEAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_SE) {
			sounds[id][i].sound.active = false;
			sounds[id][i].dpos = 0;
		}
	}
}

void SoundManager::stopVCAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_VC) {
			sounds[id][i].sound.active = false;
			sounds[id][i].dpos = 0;
		}
	}
}

void SoundManager::stopMusic(int music_kind) {
	SoundInfo sound = music[music_kind];
	stopSound(sound, 2);
}

void SoundManager::stopSound(SoundInfo sound, int id) {
	int index = findSoundIndex(sound, id);
	sounds[id][index].sound.active = false;
	sounds[id][index].dpos = 0;
}

void SoundManager::stopSoundAll() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			sounds[i][i2].sound.active = false;
			sounds[i][i2].dpos = 0;
		}
	}
}

int SoundManager::loadCommonSE(int se, int id) {
	SoundInfo sound = common_se[se];
	if (sound.name == "") {
		return -1;
	}
	loadSound(sound, id);
	return 0;
}

int SoundManager::loadCharaSE(int se, int id) {
	SoundInfo sound = getCharaSound(se, id, true);
	if (sound.name == "") {
		return -1;
	}
	loadSound(sound, id);
	return 0;
}

int SoundManager::loadVC(int vc, int id) {
	SoundInfo sound = getCharaSound(vc, id, false);
	if (sound.name == "") {
		return -1;
	}
	loadSound(sound, id);
	return 0;
}

int SoundManager::loadMusic(int music_kind) {
	SoundInfo sound = music[music_kind];
	if (sound.name == "") {
		return -1;
	}
	loadSound(sound, 2);
	return 0;
}

void SoundManager::loadSound(SoundInfo sound, int id) {
	const char* dir = (sound.dir).c_str();
	addSoundToIndex(sound, id);
}

void SoundManager::unloadCommonSE(int se, int id) {
	SoundInfo sound = common_se[se];
	unloadSound(sound, id);
}

void SoundManager::unloadCharaSE(int se, int id) {
	SoundInfo sound = getCharaSound(se, id, true);
	unloadSound(sound, id);
}

void SoundManager::unloadVC(int vc, int id) {
	SoundInfo sound = getCharaSound(vc, id, false);
	unloadSound(sound, id);
}

void SoundManager::unloadSEAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_SE) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_LockAudio();
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
			SDL_UnlockAudio();
		}
	}
}

void SoundManager::unloadVCAll(int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.sound_kind == SOUND_KIND_VC) {
			sounds[id][i].dpos = 0;
			sounds[id][i].dlen = 0;
			SDL_LockAudio();
			SDL_free(sounds[id][i].data);
			sounds[id][i].data = NULL;
			SDL_UnlockAudio();
		}
	}
}

void SoundManager::unloadMusic(int music_kind) {
	SoundInfo sound = music[music_kind];
	unloadSound(sound, 2);
}

void SoundManager::unloadSound(SoundInfo sound, int id) {
	int clear_index = findSoundIndex(sound, id);
	sounds[id][clear_index].dpos = 0;
	sounds[id][clear_index].dlen = 0;
	sounds[id][clear_index].loop_dlen = 0;
	sounds[id][clear_index].sound.active = false;
	SDL_LockAudio();
	SDL_free(sounds[id][clear_index].data);
	SDL_free(sounds[id][clear_index].loop_data);
	sounds[id][clear_index].data = NULL;
	if (sounds[id][clear_index].loop_data) {
		sounds[id][clear_index].loop_data = NULL;
	}
	SDL_UnlockAudio();
}

void SoundManager::unloadSoundAll() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (sounds[i][i2].data) {
				SDL_LockAudio();
				SDL_free(sounds[i][i2].data);
				if (sounds[i][i2].loop_data) {
					SDL_free(sounds[i][i2].loop_data);
				}

				sounds[i][i2].sound.active = false;
				sounds[i][i2].data = NULL;
				sounds[i][i2].loop_data = NULL;
				sounds[i][i2].dpos = 0;
				sounds[i][i2].dlen = 0;
				sounds[i][i2].loop_dlen = 0;
				SDL_UnlockAudio();
			}
		}
	}
}

int SoundManager::findSoundIndex(SoundInfo sound, int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (sounds[id][i].sound.dir == sound.dir) {
			return i;
		}
	}
	return MAX_SOUNDS;
}

SoundInfo SoundManager::getCharaSound(int index, int id, bool se) {
	SoundInfo sound;
	switch (battle_object_manager->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			sound = se ? roy_se[index] : roy_vc[index];
		} break;
		case(CHARA_KIND_ERIC):
		{
			sound = se ? eric_se[index] : eric_vc[index];
		} break;
		case(CHARA_KIND_LEON):
		{
			sound = se ? leon_se[index] : leon_vc[index];
		} break;
		case(CHARA_KIND_CHAMELEON):
		{
			sound = se ? chameleon_se[index] : chameleon_vc[index];
		} break;
		case(CHARA_KIND_ANGELICA):
		{
			sound = se ? angelica_se[index] : angelica_vc[index];
		} break;
		case(CHARA_KIND_NIGHTSABER):
		{
			sound = se ? nightsaber_se[index] : nightsaber_vc[index];
		} break;
		case(CHARA_KIND_SULLY):
		{
			sound = se ? sully_se[index] : sully_vc[index];
		} break;
		case(CHARA_KIND_PRIEST):
		{
			sound = se ? priest_se[index] : priest_vc[index];
		} break;
		case(CHARA_KIND_AZIEL):
		{
			sound = se ? aziel_se[index] : aziel_vc[index];
		} break;
		case(CHARA_KIND_BRUNO):
		{
			sound = se ? bruno_se[index] : bruno_vc[index];
		} break;
		case(CHARA_KIND_TESSA):
		{
			sound = se ? tessa_se[index] : tessa_vc[index];
		} break;
		case(CHARA_KIND_ALEJANDRO):
		{
			sound = se ? alejandro_se[index] : alejandro_vc[index];
		} break;
		case(CHARA_KIND_NORMAN):
		{
			sound = se ? norman_se[index] : norman_vc[index];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			sound = se ? atlas_se[index] : atlas_vc[index];
		} break;
		case(CHARA_KIND_JULIUS):
		{
			sound = se ? julius_se[index] : julius_vc[index];
		} break;
		case(CHARA_KIND_RAMONA):
		{
			sound = se ? ramona_se[index] : ramona_vc[index];
		} break;
		case(CHARA_KIND_ZYAIR):
		{
			sound = se ? zyair_se[index] : zyair_vc[index];
		} break;
		case(CHARA_KIND_VESUVIUS):
		{
			sound = se ? vesuvius_se[index] : vesuvius_vc[index];
		} break;
		default: {} break;
	}
	return sound;
}

SoundManager* SoundManager::get_instance() {
	if (instance == nullptr) {
		instance = new SoundManager;
	}
	return instance;
}

void addSoundToIndex(SoundInfo sound, int id) {
	int index;

	SDL_AudioSpec wave;
	Uint8* data;
	unsigned int dlen;
	SDL_AudioCVT cvt;
	const char* file = (sound.dir).c_str();

	SDL_AudioSpec loop_wave;
	Uint8* loop_data;
	unsigned int loop_dlen;
	SDL_AudioCVT loop_cvt;
	const char* loop_file = (sound.loop_dir).c_str();

	SoundManager* sound_manager = SoundManager::get_instance();

	for (index = 0; index < MAX_SOUNDS; index++) {
		if (!sound_manager->sounds[id][index].data) {
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
	sound_manager->sounds[id][index].data = cvt.buf;
	sound_manager->sounds[id][index].dlen = cvt.len_cvt;
	sound_manager->sounds[id][index].dpos = 0;
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

		sound_manager->sounds[id][index].loop_data = loop_cvt.buf;
		sound_manager->sounds[id][index].loop_dlen = loop_cvt.len_cvt;
		sound_manager->sounds[id][index].dpos = 0;
		SDL_UnlockAudio();
	}
	sound_manager->sounds[id][index].sound = sound; //We don't put this in until we finish, to prevent a song with a loop to play the first ~3 ticks twice
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

	for (int i = 0; i < MAX_SOUNDS; i++) {
		for (int i2 = 0; i2 < 3; i2++) {
			Sound& sound = sound_manager->sounds[i2][i];
			if (sound.sound.active) {
				diff = 0;
				data = sound.data;
				dlen = sound.dlen;
				if (sound.sound.sound_kind == SOUND_KIND_SE) {
					vol = getGameSetting("se_vol") * get_relative_one_percent(sound.sound.volume, 128);
				}
				else if (sound.sound.sound_kind == SOUND_KIND_VC) {
					vol = getGameSetting("vc_vol") * get_relative_one_percent(sound.sound.volume, 128);
				}
				else {
					vol = getGameSetting("music_vol") * get_relative_one_percent(sound.sound.volume, 128);
				}

				if (sound.sound.sound_type == SOUND_TYPE_LOOP && sound.sound.looped) { 
					data = sound.loop_data;
					dlen = sound.loop_dlen;
				}

				source = (Uint8*)SDL_malloc(len); //Allocate enough memory to hold our sound data
				if (sound.dpos + len > dlen) { //Check if we're about to hit the end of the file, and if so by how much
					diff = (sound.dpos + len) - dlen;
				}

				mlen = clamp(0, len, dlen - sound.dpos);

				//Copy as much data from the audio track as we have into the source variable, making sure not to copy data that doesn't exist.

				SDL_memcpy(source, &data[sound.dpos], mlen);
	
				sound.dpos += len; //Add the length of the stream to the audio's position.

				if (diff != 0) { //If we went over
					if (sound.sound.sound_type == SOUND_TYPE_LOOP) { 
						if (!sound.sound.looped) { //If we've never looped before, clarify that we're getting the rest of our
							//data from the looped version, and that we are now in the loop state.
							sound.sound.looped = true;
							data = sound.loop_data;
						}
						sound.dpos = 0;
						SDL_memcpy(&source[len - diff], &data[sound.dpos], diff);
						sound.dpos += diff;

					}
					else { //Otherwise just get that shit outta here
						sound.sound.active = false;
						sound.dpos = 0;
					}
				}
				SDL_MixAudio(stream, source, mlen, vol);
				SDL_free(source);
			}
		}
	}
}
