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

void SoundManager::hyperInit() { 
	//We will put the directories for every piece of audio in the entire game with this function, hence the name
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

int SoundManager::playStageMusic(int stage_kind) {
	string dir = "resource/sound/bgm/stage/" + stage_music[stage_kind] + ".wav";
	if (stage_music[stage_kind] == "") {
		return -1;
	}
	playSound(dir, 2);
	return 0;
}

int SoundManager::playMusic(int music) {
	return 0;
}

void SoundManager::playSound(string file, int id) {
	int index = MAX_SOUNDS;
	addSoundToIndex((char*)file.c_str(), &index, id);
	if (index != MAX_SOUNDS) {
		active_sounds[id][index] = file;
	}
}

int SoundManager::findSoundIndex(string file, int id) {
	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (active_sounds[id][i] == file) {
			cout << i << endl;
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