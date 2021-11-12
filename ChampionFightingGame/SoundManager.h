#pragma once
#include "SDL_audio.h"
#include "FighterAccessor.h"
#include "Fighter.h"
#include "utils.h"

class Sound {
public:
	Sound();
	Sound(string name, int sound_kind, int chara_kind = CHARA_KIND_MAX, int sound_type = SOUND_TYPE_NORMAL, int loop_point = 0);

	string name;
	string dir;
	int loop_point;
	int sound_kind;
	int sound_type;
};

class SoundManager {
public:
	SoundManager();
	SoundManager(bool initiate);
	FighterAccessor *fighter_accessor;

	Sound active_sounds[3][MAX_SOUNDS];

	Sound common_se[COMMON_SE_MAX];
	Sound roy_se[ROY_SE_MAX];
	Sound roy_vc[ROY_VC_MAX];
	Sound eric_se[ERIC_SE_MAX];
	Sound eric_vc[ERIC_VC_MAX];
	Sound atlas_se[ATLAS_SE_MAX];
	Sound atlas_vc[ATLAS_VC_MAX];
	Sound stage_music[STAGE_MUSIC_MAX];
	Sound music[MUSIC_KIND_MAX];

	int stage_loop_point[STAGE_MUSIC_MAX] = { 0 };
	int music_loop_point[MUSIC_KIND_MAX] = { 0 };

	void hyperInit();

	int playCommonSE(int se, int id);
	int playCharaSE(int se, int id);
	int playVC(int voice, int id);
	int playStageMusic(int stage_kind);
	int playMusic(int music_kind);
	void playSound(Sound sound, int id);
	void endCommonSE(int se, int id);
	void endCharaSE(int se, int id);
	void endVC(int voice, int id);
	void endSEAll(int id);
	void endVCAll(int id);
	void endStageMusic(int stage_kind);
	void endMusic(int music_kind);
	int findSoundIndex(Sound sound, int id);
	void endSound(Sound sound, int id);
	void endSoundAll();
	void checkSoundEnd();
};