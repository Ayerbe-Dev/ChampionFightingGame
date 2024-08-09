#include "SaveManager.h"
#include <fstream>

SaveManager::SaveManager() {
	load_save_data();
	load_player_info();
}

void SaveManager::load_save_data() {
	std::ifstream settings_file;
	settings_file.open("resource/data/saved_data.yml");
	if (settings_file.fail()) {
		std::cout << "Failed to open saved data file!\n";
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
	settings_file.open("resource/data/unsaved_data.yml");
	if (settings_file.fail()) {
		std::cout << "Failed to open unsaved data file!\n";
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
	if (settings_map.contains(setting)) {
		settings[settings_map[setting]].val = val;
		return;
	}
	if (unsaved_settings_map.contains(setting)) {
		unsaved_settings[unsaved_settings_map[setting]].val = val;
	}
}

void SaveManager::update_save_data() {
	std::ofstream settings_file;
	settings_file.open("resource/data/saved_data.yml", std::ofstream::trunc);
	if (settings_file.fail()) {
		std::cout << "Failed to open saved data file!\n";
		settings_file.close();
		return;
	}
	for (int i = 0, max = settings.size(); i < max; i++) {
		settings_file << settings[i].name << " " << settings[i].val << "\n";
	}
	settings_file.close();
}

void SaveManager::load_player_info() {
	std::ifstream player_info_file;
	player_info_file.open("resource/data/player_info.yml");
	if (player_info_file.fail()) {
		std::cout << "Failed to open Player Info file!\n";
		player_info_file.close();
		return;
	}
	std::string name;
	while (player_info_file >> name && name != "") {
		int num_custom_mappings;
		PlayerInfo new_player_info(name);
		player_info_file >> new_player_info.control_type >> num_custom_mappings;
		for (int i = 0; i < num_custom_mappings; i++) {
			Button b;
			player_info_file >> b.button_kind >> b.k_mapping >> b.c_mapping >> b.c_axis;
			new_player_info.custom_mappings.push_back(b);
		}
		player_info_file >> new_player_info.preferred_chara;
		for (int i = 0; i < CHARA_KIND_MAX; i++) {
			player_info_file >> new_player_info.preferred_costume[i] >> new_player_info.preferred_color[i];
		}
		player_info[name] = new_player_info;
		player_info_ordered.push_back(&player_info[name]);
	}
	
	player_info_file.close();
}

int SaveManager::add_player_info(std::string name) {
	if (player_info.contains(name)) {
		PlayerInfo* target = &player_info[name];
		for (int i = 0, max = player_info_ordered.size(); i < max; i++) {
			if (player_info_ordered[i] == target) {
				return i;
			}
		}
	}
	PlayerInfo new_player_info(name);
	player_info[name] = new_player_info;
	player_info_ordered.push_front(&new_player_info);
	return 0;
}

PlayerInfo* SaveManager::get_player_info(int index) {
	if (index == -1) return &default_player_info;
	if (player_info.size() <= (size_t)index) return nullptr;
	return player_info_ordered[index];
}

int SaveManager::get_num_player_info() {
	return player_info_ordered.size();
}

void SaveManager::sort_player_info(PlayerInfo* player_info) {
	if (player_info == &default_player_info) return;
	for (int i = 0, max = player_info_ordered.size(); i < max; i++) {
		if (player_info_ordered[i] == player_info) {
			std::deque<PlayerInfo*>::iterator it = std::next(player_info_ordered.begin(), i);
			player_info_ordered.erase(it);
			break;
		}
	}
	player_info_ordered.push_front(player_info);
}

void SaveManager::remove_player_info(int index) {
	std::deque<PlayerInfo*>::iterator it = std::next(player_info_ordered.begin(), index);
	std::string name = (*it)->name;
	player_info_ordered.erase(it);
	player_info.erase(name);
}

void SaveManager::update_player_info() {
	std::ofstream player_info_file;
	player_info_file.open("resource/data/player_info.yml", std::ofstream::trunc);
	if (player_info_file.fail()) {
		std::cout << "Failed to open Player Info file!\n";
		player_info_file.close();
		return;
	}
	for (int i = 0, max = player_info_ordered.size(); i < max; i++) {
		player_info_file << player_info_ordered[i]->name << " " 
			<< player_info_ordered[i]->control_type << " "
			<< player_info_ordered[i]->custom_mappings.size() << " ";
		for (int i2 = 0, max2 = player_info_ordered[i]->custom_mappings.size(); 
			i2 < max2; i2++) {
			player_info_file << player_info_ordered[i]->custom_mappings[i2].button_kind << " "
				<< player_info_ordered[i]->custom_mappings[i2].k_mapping << " "
				<< player_info_ordered[i]->custom_mappings[i2].c_mapping << " "
				<< player_info_ordered[i]->custom_mappings[i2].c_axis << " ";
		}
		player_info_file << player_info_ordered[i]->preferred_chara << " ";
		for (int i2 = 0; i2 < CHARA_KIND_MAX; i2++) {
			player_info_file << player_info_ordered[i]->preferred_costume[i2] << " "
				<< player_info_ordered[i]->preferred_color[i2] << " ";
		}
		player_info_file << "\n";
	}
	player_info_file.close();
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