#include "SaveManager.h"
#include <fstream>

SaveManager::SaveManager() {
	load_game_settings();
}

void SaveManager::load_game_settings() {
	std::ifstream settings_file;
	settings_file.open("resource/save/game_settings.yml");
	if (settings_file.fail()) {
		std::cout << "Failed to open settings file! \n";
		settings_file.close();
		return;
	}
	std::string name;
	int val;
	for (int i = 0; settings_file >> name; i++) {
		settings_file >> val;
		GameSetting setting(name, val);
		settings.push_back(setting);
		settings_map[name] = i; 
	}
	settings_file.close();
}

int SaveManager::get_game_setting(std::string setting) {
	if (settings_map.find(setting) == settings_map.end()) {
		return 0;
	}
	else {
		return settings[settings_map[setting]].val;
	}
}

void SaveManager::set_game_setting(std::string setting, int val) {
	if (settings_map.find(setting) == settings_map.end()) {
		return;
	}
	settings[settings_map[setting]].val = val;
}

void SaveManager::update_game_settings() {
	std::ofstream settings_file;
	settings_file.open("resource/save/game_settings.yml", std::ofstream::trunc);
	if (settings_file.fail()) {
		std::cout << "Failed to open settings file! \n";
		settings_file.close();
		return;
	}
	for (int i = 0, max = settings.size(); i < max; i++) {
		settings_file << settings[i].name << " " << settings[i].val << "\n";
	}
	settings_file.close();
}

GameSetting::GameSetting() {
	name = "";
	val = 0;
}

GameSetting::GameSetting(std::string name, int val) {
	this->name = name;
	this->val = val;
}

SaveManager* SaveManager::instance = nullptr;
SaveManager* SaveManager::get_instance() {
	if (instance == nullptr) {
		instance = new SaveManager;
	}
	return instance;
}

void SaveManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}