#pragma once
#include "SDL_audio.h"
#include "utils.h"

class SoundManager {
public:
	SoundManager();
	SoundManager(bool initiate);
	string active_sounds[3][MAX_SOUNDS];

	string common_se[COMMON_SE_MAX];
	string roy_se[ROY_SE_MAX];
	string roy_voice[ROY_VOICE_MAX];
	string eric_se[ERIC_SE_MAX];
	string eric_voice[ERIC_VOICE_MAX];
	string atlas_se[ATLAS_SE_MAX];
	string atlas_voice[ATLAS_VOICE_MAX];
	string stage_music[STAGE_MUSIC_MAX];
	string music[MUSIC_KIND_MAX];

	void hyperInit();

	int playCommonSE(int se, int id);
	int playCharaSE(int se, int chara_kind, int id);
	int playVoice(int voice, int chara_kind, int id);
	int playStageMusic(int stage_kind);
	int playMusic(int music);
	void playSound(string file, int id);
	int findSoundIndex(string file, int id);
	void endSound(string file, int id);
	void endSoundAll();
};