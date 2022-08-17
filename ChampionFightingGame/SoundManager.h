#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <unordered_map>
#include "Sound.h"

class SoundManager {
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	void play_sound(int object_id, int sound_kind, std::string name, int volume = -1);

	void pause_sound_all(int object_id = -1, int sound_kind = -1);
	void resume_sound_all(int object_id = -1, int sound_kind = -1);

	void stop_sound(int object_id, int sound_kind, std::string name);
	void stop_sound_all(int object_id = -1, int sound_kind = -1);

	void load_sound(std::string name);
	void unload_sound(std::string name);
	void unload_all_sounds();

	int add_sound_player();
	void add_sound_player(int object_id);
	void remove_sound_player(int id);
	void remove_sound_players();

	std::vector<std::array<std::list<SoundInstance>, SOUND_KIND_MAX>> active_sounds;

	static SoundManager* get_instance();
	void destroy_instance();
private:
	SoundManager();
	void populate_sounds();
	void add_sound_info(std::string name, std::string dir, int sound_kind, int sound_type, int volume = 32);

	std::vector<SoundInfo> sound_info;
	std::unordered_map<std::string, int> sound_info_map;

	std::vector<Sound> loaded_sounds;
	std::unordered_map<std::string, int> sound_name_map;

	std::unordered_map<int, int> id2index;

	static SoundManager* instance;
};

void audio_callback(void* unused, Uint8* stream, int len);