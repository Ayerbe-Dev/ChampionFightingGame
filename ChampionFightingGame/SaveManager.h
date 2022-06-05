#pragma once
#include <iostream>
#include <vector>
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

	std::vector<PlayerInfo> player_info;

	void load_game_settings();
	int get_game_setting(std::string setting);
	void set_game_setting(std::string setting, int val);
	void update_game_settings();

	static SaveManager* get_instance();
	void destroy_instance();
private:
	SaveManager();

	std::vector<GameSetting> settings;
	std::map<std::string, int> settings_map;

	static SaveManager* instance;
};

enum {
	MUSIC_SETTING_STAGE,
	MUSIC_SETTING_CHARA,
	MUSIC_SETTING_PLAYER,
};