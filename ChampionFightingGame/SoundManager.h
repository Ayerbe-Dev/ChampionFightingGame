#pragma once
#include "SDL_audio.h"
#include "FighterAccessor.h"
#include "Fighter.h"
#include "utils.h"


class Sound {
public:
	Sound();
	Sound(string name, int sound_kind, int chara_kind = CHARA_KIND_MAX, int volume = 32, int sound_type = SOUND_TYPE_NORMAL);

	string name;
	string dir;
	string loop_dir;
	int sound_kind;
	int sound_type;
	int volume;
	bool looped;
	bool active;
};

struct SoundInfo {
	u8* data;
	u8* loop_data;
	u32 dpos;
	u32 dlen;
	u32 loop_dlen;
	Sound sound;
};

class SoundManager {
public:
	SoundManager();
	SoundManager(bool initiate);
	FighterAccessor *fighter_accessor;

	Sound common_se[COMMON_SE_MAX];
	Sound roy_se[ROY_SE_MAX];
	Sound roy_vc[ROY_VC_MAX];
	Sound eric_se[ERIC_SE_MAX];
	Sound eric_vc[ERIC_VC_MAX];
	Sound atlas_se[ATLAS_SE_MAX];
	Sound atlas_vc[ATLAS_VC_MAX];
	Sound music[MUSIC_KIND_MAX];

	void hyperInit();

	//Play Sound funcs; The sounds themselves are loaded ahead of time, so these actually just mark a sound as active. Note: If a sound is paused,
	//this series will not start a sound from the beginning

	void playCommonSE(int se, int id);
	void playCharaSE(int se, int id);
	void playVC(int voice, int id);
	void playMusic(int music_kind);
	void playSound(Sound sound, int id);

	//Mark the sound as inactive, but unlike the stop functions, this series doesn't set the position of a sound back to 0

	void pauseCommonSE(int se, int id);
	void pauseCharaSE(int se, int id);
	void pauseVC(int voice, int id);
	void pauseSEAll(int id);
	void pauseVCAll(int id);
	void pauseMusic(int music_kind);
	void pauseSound(Sound sound, int id);
	void pauseSoundAll();

	//Resume any paused sounds. There's no function to resume a specific sound because we can already use the playSound series for that

	void resumeSEAll(int id);
	void resumeVCAll(int id);
	void resumeMusicAll();
	void resumeSoundAll(int id);

	//Stop a sound effect, marking it as inactive and resetting its position for the next time it's used

	void stopCommonSE(int se, int id);
	void stopCharaSE(int se, int id);
	void stopVC(int voice, int id);
	void stopSEAll(int id);
	void stopVCAll(int id);
	void stopMusic(int music_kind);
	void stopSound(Sound sound, int id);
	void stopSoundAll();

	int loadCommonSE(int se, int id);
	int loadCharaSE(int se, int id);
	int loadVC(int voice, int id);
	int loadStageMusic(int stage_kind);
	int loadMusic(int music_kind);
	void loadSound(Sound sound, int id);
	void unloadCommonSE(int se, int id);
	void unloadCharaSE(int se, int id);
	void unloadVC(int voice, int id);
	void unloadSEAll(int id);
	void unloadVCAll(int id);
	void unloadStageMusic(int stage_kind);
	void unloadMusic(int music_kind);
	void unloadSound(Sound sound, int id);
	void unloadSoundAll();

	int findSoundIndex(Sound sound, int id);
	Sound getCharaSound(int index, int id, bool se);
};

void audio_callback(void* unused, Uint8* stream, int len);
void addSoundToIndex(Sound sound, int id);