#pragma once
#include <iostream>
#include "BattleObjectManager.h"
#include "Fighter.h"
#include "CharaKind.h"
#include "Sound.h"

class SoundManager {
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	BattleObjectManager *battle_object_manager;

	SoundInfo common_se[COMMON_SE_MAX];
	SoundInfo roy_se[ROY_SE_MAX];
	SoundInfo roy_vc[ROY_VC_MAX];
	SoundInfo eric_se[ERIC_SE_MAX];
	SoundInfo eric_vc[ERIC_VC_MAX];
	SoundInfo leon_se[LEON_SE_MAX];
	SoundInfo leon_vc[LEON_VC_MAX];
	SoundInfo chameleon_se[CHAMELEON_SE_MAX];
	SoundInfo chameleon_vc[CHAMELEON_VC_MAX];
	SoundInfo angelica_se[ANGELICA_SE_MAX];
	SoundInfo angelica_vc[ANGELICA_VC_MAX];
	SoundInfo nightsaber_se[NIGHTSABER_SE_MAX];
	SoundInfo nightsaber_vc[NIGHTSABER_VC_MAX];
	SoundInfo sully_se[SULLY_SE_MAX];
	SoundInfo sully_vc[SULLY_VC_MAX];
	SoundInfo priest_se[PRIEST_SE_MAX];
	SoundInfo priest_vc[PRIEST_VC_MAX];
	SoundInfo aziel_se[AZIEL_SE_MAX];
	SoundInfo aziel_vc[AZIEL_VC_MAX];
	SoundInfo bruno_se[BRUNO_SE_MAX];
	SoundInfo bruno_vc[BRUNO_VC_MAX];
	SoundInfo tessa_se[TESSA_SE_MAX];
	SoundInfo tessa_vc[TESSA_VC_MAX];
	SoundInfo alejandro_se[ALEJANDRO_SE_MAX];
	SoundInfo alejandro_vc[ALEJANDRO_VC_MAX];
	SoundInfo norman_se[NORMAN_SE_MAX];
	SoundInfo norman_vc[NORMAN_VC_MAX];
	SoundInfo atlas_se[ATLAS_SE_MAX];
	SoundInfo atlas_vc[ATLAS_VC_MAX];
	SoundInfo julius_se[JULIUS_SE_MAX];
	SoundInfo julius_vc[JULIUS_VC_MAX];
	SoundInfo ramona_se[RAMONA_SE_MAX];
	SoundInfo ramona_vc[RAMONA_VC_MAX];
	SoundInfo zyair_se[ZYAIR_SE_MAX];
	SoundInfo zyair_vc[ZYAIR_VC_MAX];
	SoundInfo vesuvius_se[VESUVIUS_SE_MAX];
	SoundInfo vesuvius_vc[VESUVIUS_VC_MAX];
	SoundInfo music[MUSIC_KIND_MAX];

	void hyperInit();

	//Play Sound funcs; The sounds themselves are loaded ahead of time, so these actually just mark a sound as active. Note: If a sound is paused,
	//this series will not start a sound from the beginning

	void playCommonSE(int se, int id);
	void playCharaSE(int se, int id);
	void playVC(int voice, int id);
	void playMusic(int music_kind);
	void playSound(SoundInfo sound, int id);

	//Mark the sound as inactive, but unlike the stop functions, this series doesn't set the position of a sound back to 0

	void pauseCommonSE(int se, int id);
	void pauseCharaSE(int se, int id);
	void pauseVC(int voice, int id);
	void pauseSEAll(int id);
	void pauseVCAll(int id);
	void pauseMusic(int music_kind);
	void pauseSound(SoundInfo sound, int id);
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
	void stopSound(SoundInfo sound, int id);
	void stopSoundAll();

	int loadCommonSE(int se, int id);
	int loadCharaSE(int se, int id);
	int loadVC(int voice, int id);
	int loadMusic(int music_kind);
	void loadSound(SoundInfo sound, int id);
	void unloadCommonSE(int se, int id);
	void unloadCharaSE(int se, int id);
	void unloadVC(int voice, int id);
	void unloadSEAll(int id);
	void unloadVCAll(int id);
	void unloadMusic(int music_kind);
	void unloadSound(SoundInfo sound, int id);
	void unloadSoundAll();

	int findSoundIndex(SoundInfo sound, int id);
	SoundInfo getCharaSound(int index, int id, bool se);
	Sound sounds[3][MAX_SOUNDS];

	static SoundManager* get_instance();
private:
	SoundManager();
	static SoundManager* instance;
};

void audio_callback(void* unused, Uint8* stream, int len);
void addSoundToIndex(SoundInfo sound, int id);