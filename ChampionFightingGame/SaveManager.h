#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include "PlayerInfo.h"

struct GameSetting {
	GameSetting();
	GameSetting(std::string name, int val);
	std::string name;
	int val;
};

class SaveManager {
public:
	SaveManager(SaveManager& other) = delete;
	void operator=(const SaveManager& other) = delete;

	void load_save_data();
	int get_game_setting(std::string setting);
	void set_game_setting(std::string setting, int val);
	void update_save_data();

	void load_player_info();
	int add_player_info(std::string name);
	PlayerInfo* get_player_info(int index);
	int get_num_player_info();
	void sort_player_info(PlayerInfo* player_info);
	void remove_player_info(int index);
	void update_player_info();

	static SaveManager* get_instance();
	void destroy_instance();
private:
	SaveManager();

	std::vector<GameSetting> settings;
	std::vector<GameSetting> unsaved_settings;
	std::map<std::string, int> settings_map;
	std::map<std::string, int> unsaved_settings_map;
	std::map<std::string, PlayerInfo> player_info;
	std::deque<PlayerInfo*> player_info_ordered;
	PlayerInfo default_player_info;

	static SaveManager* instance;
};

enum {
	MUSIC_SETTING_STAGE,
	MUSIC_SETTING_CHARA,
	MUSIC_SETTING_PLAYER,
};