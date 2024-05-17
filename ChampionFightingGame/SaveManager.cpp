#include "SaveManager.h"
#include <fstream>

SaveManager::SaveManager() {
	load_game_settings();
	load_player_info();
}

void SaveManager::load_game_settings() {
	std::ifstream settings_file;
	settings_file.open("resource/save/saved_settings.yml");
	if (settings_file.fail()) {
		std::cout << "Failed to open saved settings file!\n";
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
	settings_file.open("resource/save/unsaved_settings.yml");
	if (settings_file.fail()) {
		std::cout << "Failed to open unsaved settings file!\n";
		settings_file.close();
		return;
	}
	for (int i = 0; settings_file >> name; i++) {
		settings_file >> val;
		GameSetting setting(name, val);
		unsaved_settings.push_back(setting);
		unsaved_settings_map[name] = i;
	}
	settings_file.close();
}

int SaveManager::get_game_setting(std::string setting) {
	if (settings_map.contains(setting)) {
		return settings[settings_map[setting]].val;
	}
	if (unsaved_settings_map.contains(setting)) {
		return unsaved_settings[unsaved_settings_map[setting]].val;
	}
	return 0;
}

void SaveManager::set_game_setting(std::string setting, int val) {
	if (settings_map.find(setting) == settings_map.end()) {
		return;
	}
	settings[settings_map[setting]].val = val;
}

void SaveManager::save_game_settings() {
	std::ofstream settings_file;
	settings_file.open("resource/save/saved_settings.yml", std::ofstream::trunc);
	if (settings_file.fail()) {
		std::cout << "Failed to open saved settings file!\n";
		settings_file.close();
		return;
	}
	for (int i = 0, max = settings.size(); i < max; i++) {
		settings_file << settings[i].name << " " << settings[i].val << "\n";
	}
	settings_file.close();
}

void SaveManager::load_player_info() {

}

int SaveManager::add_player_info(std::string name) {
	if (player_info.contains(name)) {
		int ret = 0;
		PlayerInfo* target = &player_info[name];
		for (std::list<PlayerInfo*>::iterator it = player_info_ordered.begin(), 
			max = player_info_ordered.end(); it != max; it++) {
			if (*it == target) {
				return ret;
			}
			ret++;
		}
	}
	PlayerInfo new_player_info(name);
	player_info[name] = new_player_info;
	player_info_ordered.push_front(&new_player_info);
	return 0;
}

PlayerInfo* SaveManager::get_player_info(int index) {
	if (index == -1) return &default_player_info;
	if (player_info.size() <= (size_t)index) {
		return nullptr;
	}
	std::list<PlayerInfo*>::iterator it = std::next(player_info_ordered.begin(), index);
	PlayerInfo* ret = *it;
	player_info_ordered.erase(it);
	player_info_ordered.push_front(ret);
	return ret;
}

void SaveManager::remove_player_info(int index) {
	std::list<PlayerInfo*>::iterator it = std::next(player_info_ordered.begin(), index);
	std::string name = (*it)->name;
	player_info_ordered.erase(it);
	player_info.erase(name);
}

void SaveManager::update_player_info() {

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
	update_player_info();
	if (instance != nullptr) {
		delete instance;
	}
}