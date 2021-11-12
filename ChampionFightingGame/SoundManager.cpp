#include "SoundManager.h"
extern SoundInfo sounds[3][MAX_SOUNDS];

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

void SoundManager::hyperInit() {//The string names for all audio go here
	stage_music[STAGE_MUSIC_ATLAS] = "Atlas_Theme";

}

int SoundManager::playCommonSE(int se, int id) {
	string file = common_se[se];
	string dir = "resource/sound/se/common/" + file + ".wav";
	if (file == "") {
		return -1;
	}
	playSound(dir, id);
	return 0; 
}

int SoundManager::playCharaSE(int se, int id) {
	string file = "";
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY): {
			file = roy_se[se];
		} break;
		case(CHARA_KIND_ERIC):{
			file = eric_se[se];
		} break;
		case(CHARA_KIND_ATLAS): {
			file = atlas_se[se];
		} break;
		default: {} break;
	}
	if (file == "") {
		return -1;
	}
	string dir = "resource/sound/se/" + fighter_accessor->fighter[id]->chara_name + "/" + file + ".wav";
	playSound(dir, id);
	return 0;
}

int SoundManager::playVoice(int voice, int id) {
	string file = "";
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			file = roy_voice[voice];
		} break;
		case(CHARA_KIND_ERIC):
		{
			file = eric_voice[voice];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			file = atlas_voice[voice];
		} break;
		default: {} break;
	}
	if (file == "") {
		return -1;
	}
	string dir = "resource/sound/voice/" + fighter_accessor->fighter[id]->chara_name + "/" + file + ".wav";
	playSound(dir, id);
	return 0;
}

int SoundManager::playStageMusic(int stage_kind) {
	string file = stage_music[stage_kind];
	string dir = "resource/sound/bgm/stage/" + file + ".wav";
	if (file == "") {
		return -1;
	}
	playSound(dir, 2);
	return 0;
}

int SoundManager::playMusic(int music_kind) {
	string file = music[music_kind];
	string dir = "resource/sound/bgm/common/" + file + ".wav";
	if (file == "") {
		return -1;
	}
	playSound(dir, 2);
	return 0;
}

/*
	Create a pointer to figure out where the sound will be put on the list, then pass it to a real sound function over in utils. 
	
	To clarify, the reason that the actual addSoundToIndex function is not a member of SoundManager is because it would require some things to be 
	defined in this file rather than in utils, and doing that makes it so the audio_callback function doesn't work because of some scope shenanigans.

	If you can figure out how to make that work then go ahead, personally I think this approach is ok
*/
void SoundManager::playSound(string dir, int id) {
	int index = 0;
	addSoundToIndex((char*)dir.c_str(), &index, id);
	if (index != MAX_SOUNDS) {
		active_sounds[id][index] = dir;
	}
}

int SoundManager::findSoundIndex(string file, int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (active_sounds[id][i] == file) {
			return i;
		}
	}
	return MAX_SOUNDS;
}

void SoundManager::endCommonSE(int se, int id) {
	string file = common_se[se];
	string dir = "resource/sound/se/common/" + file + ".wav";
	endSound(dir, id);
}

void SoundManager::endCharaSE(int se, int id) {
	string file = "";
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			file = roy_se[se];
		} break;
		case(CHARA_KIND_ERIC):
		{
			file = eric_se[se];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			file = atlas_se[se];
		} break;
		default: {} break;
	}
	string dir = "resource/sound/se/" + fighter_accessor->fighter[id]->chara_name + "/" + file + ".wav";
	endSound(dir, id);
}

void SoundManager::endVoice(int voice, int id) {
	string file = "";
	switch (fighter_accessor->fighter[id]->chara_kind) {
		case(CHARA_KIND_ROY):
		{
			file = roy_voice[voice];
		} break;
		case(CHARA_KIND_ERIC):
		{
			file = eric_voice[voice];
		} break;
		case(CHARA_KIND_ATLAS):
		{
			file = atlas_voice[voice];
		} break;
		default: {} break;
	}
	string dir = "resource/sound/voice/" + fighter_accessor->fighter[id]->chara_name + "/" + file + ".wav";
	endSound(dir, id);
}

void SoundManager::endStageMusic(int stage_kind) {
	string file = stage_music[stage_kind];
	string dir = "resource/sound/bgm/stage/" + file + ".wav";
	endSound(dir, 2);
}

void SoundManager::endMusic(int music_kind) {
	string file = music[music_kind];
	string dir = "resource/sound/bgm/common/" + file + ".wav";
	endSound(dir, 2);
}

void SoundManager::endSound(string dir, int id) {
	int clear_index = findSoundIndex(dir, id);
	sounds[id][clear_index].dpos = 0;
	sounds[id][clear_index].dlen = 0;
	free(sounds[id][clear_index].data);
	sounds[id][clear_index].data = NULL;
}

void SoundManager::endSoundAll() {
	SDL_LockAudio();
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			if (sounds[i][i2].data) {
				free(sounds[i][i2].data); //Why the hell does this give me a read access violation

				//As in, NOT a write access violation

				//lmao?????
				sounds[i][i2].data = NULL;
			}
			sounds[i][i2].dpos = 0;
			sounds[i][i2].dlen = 0;
		}
	}
	SDL_UnlockAudio();
}