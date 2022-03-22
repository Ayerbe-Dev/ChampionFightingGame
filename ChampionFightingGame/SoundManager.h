#pragma once
#include <iostream>
#include <SDL/SDL_audio.h>
#include "BattleObjectManager.h"
#include "Fighter.h"
#include "CharaKind.h"
#include "SoundConstants.h"

class Sound {
public:
	Sound();
	Sound(std::string name, int sound_kind, int chara_kind = CHARA_KIND_MAX, int volume = 32, int sound_type = SOUND_TYPE_NORMAL);

	std::string name;
	std::string dir;
	std::string loop_dir;
	int sound_kind;
	int sound_type;
	int volume;
	bool looped;
	bool active;
};

struct SoundInfo {
	Uint8* data;
	Uint8* loop_data;
	unsigned int dpos;
	unsigned int dlen;
	unsigned int loop_dlen;
	Sound sound;
};

class SoundManager {
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	BattleObjectManager *battle_object_manager;

	Sound common_se[COMMON_SE_MAX];
	Sound roy_se[ROY_SE_MAX];
	Sound roy_vc[ROY_VC_MAX];
	Sound eric_se[ERIC_SE_MAX];
	Sound eric_vc[ERIC_VC_MAX];
	Sound leon_se[LEON_SE_MAX];
	Sound leon_vc[LEON_VC_MAX];
	Sound chameleon_se[CHAMELEON_SE_MAX];
	Sound chameleon_vc[CHAMELEON_VC_MAX];
	Sound angelica_se[ANGELICA_SE_MAX];
	Sound angelica_vc[ANGELICA_VC_MAX];
	Sound nightsaber_se[NIGHTSABER_SE_MAX];
	Sound nightsaber_vc[NIGHTSABER_VC_MAX];
	Sound sully_se[SULLY_SE_MAX];
	Sound sully_vc[SULLY_VC_MAX];
	Sound priest_se[PRIEST_SE_MAX];
	Sound priest_vc[PRIEST_VC_MAX];
	Sound aziel_se[AZIEL_SE_MAX];
	Sound aziel_vc[AZIEL_VC_MAX];
	Sound bruno_se[BRUNO_SE_MAX];
	Sound bruno_vc[BRUNO_VC_MAX];
	Sound tessa_se[TESSA_SE_MAX];
	Sound tessa_vc[TESSA_VC_MAX];
	Sound alejandro_se[ALEJANDRO_SE_MAX];
	Sound alejandro_vc[ALEJANDRO_VC_MAX];
	Sound norman_se[NORMAN_SE_MAX];
	Sound norman_vc[NORMAN_VC_MAX];
	Sound atlas_se[ATLAS_SE_MAX];
	Sound atlas_vc[ATLAS_VC_MAX];
	Sound julius_se[JULIUS_SE_MAX];
	Sound julius_vc[JULIUS_VC_MAX];
	Sound ramona_se[RAMONA_SE_MAX];
	Sound ramona_vc[RAMONA_VC_MAX];
	Sound zyair_se[ZYAIR_SE_MAX];
	Sound zyair_vc[ZYAIR_VC_MAX];
	Sound vesuvius_se[VESUVIUS_SE_MAX];
	Sound vesuvius_vc[VESUVIUS_VC_MAX];
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
	int loadMusic(int music_kind);
	void loadSound(Sound sound, int id);
	void unloadCommonSE(int se, int id);
	void unloadCharaSE(int se, int id);
	void unloadVC(int voice, int id);
	void unloadSEAll(int id);
	void unloadVCAll(int id);
	void unloadMusic(int music_kind);
	void unloadSound(Sound sound, int id);
	void unloadSoundAll();

	int findSoundIndex(Sound sound, int id);
	Sound getCharaSound(int index, int id, bool se);

	static SoundManager* get_instance();
private:
	SoundManager();
	static SoundManager* instance;
};

void audio_callback(void* unused, Uint8* stream, int len);
void addSoundToIndex(Sound sound, int id);