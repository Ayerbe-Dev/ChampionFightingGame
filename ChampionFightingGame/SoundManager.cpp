#include "SoundManager.h"
extern SoundInfo sounds[2][MAX_SOUNDS];

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
	return 0;
}

int SoundManager::playCharaSE(int se, int chara_kind, int id) {
	return 0;
}

int SoundManager::playVoice(int voice, int chara_kind, int id) {
	return 0;
}

int SoundManager::playStageMusic(int stage_kind) { //Determine the full directory for the audio we want, then call playSound with it
	string dir = "resource/sound/bgm/stage/" + stage_music[stage_kind] + ".wav";
	if (stage_music[stage_kind] == "") {
		return -1;
	}
	playSound(dir, 2);
	return 0;
}

int SoundManager::playMusic(int music, int id) {
	return 0;
}

/*
	Create a pointer to figure out where the sound will be put on the list, then pass it to a real sound function over in utils. 
	
	To clarify, the reason that the actual addSoundToIndex function is not a member of SoundManager is because it would require some things to be 
	defined in this file rather than in utils, and doing that makes it so the audio_callback function doesn't work because of some scope shenanigans.

	If you can figure out how to make that work then go ahead, personally I think this approach is ok
*/
void SoundManager::playSound(string file, int id) {
	int index = 0;
	addSoundToIndex((char*)file.c_str(), &index, id);
	if (index != MAX_SOUNDS) {
		active_sounds[id][index] = file;
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

void SoundManager::endSound(string file, int id) {
	int clear_index = findSoundIndex(file, id);
	sounds[id][clear_index].dpos = 0;
	sounds[id][clear_index].dlen = 0;
	sounds[id][clear_index].data = 0;
}

void SoundManager::endSoundAll() {
	for (int i = 0; i < 3; i++) {
		for (int i2 = 0; i2 < MAX_SOUNDS; i2++) {
			sounds[i][i2].dpos = 0;
			sounds[i][i2].dlen = 0;
			sounds[i][i2].data = 0;
		}
	}
}