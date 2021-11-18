#include "GameSettings.h"

GameSettings::GameSettings() {
	ifstream settings;
	settings.open("resource/save/game_settings.yml");
	for (int i = 0; i < SETTING_KIND_MAX; i++) {
		settings >> this->settings[i].name >> this->settings[i].val;
	}
	settings.close();
}