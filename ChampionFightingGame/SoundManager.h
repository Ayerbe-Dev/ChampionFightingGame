#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <map>
#include "Sound.h"

class GameObject;

struct MusicInstance {
	unsigned int cursor;
	char curr_track;
	unsigned int source;
	std::string name;
};

class SoundManager {
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	void register_game_object(GameObject* game_object);
	void clear_game_objects();

	void process_sounds();

	void play_music(std::string name);
	void pause_music(std::string name);
	void pause_music_all();
	void resume_music(std::string name);
	void resume_music_all();
	void stop_music(std::string name);
	void stop_music_all();
;
	void pause_vc_all();
	void pause_se_all();
	void resume_vc_all();
	void resume_se_all();

	Sound get_sound(std::string name);
	unsigned int get_sound_buffer(std::string name);

	void load_sound(std::string name, std::string dir, float volume_mod);
	void load_music(std::string name, std::string dir, float volume_mod);
	void unload_sound(std::string name);
	void unload_music(std::string name);
	void unload_all_sounds();
	void unload_all_music();

	static SoundManager* get_instance();
	void destroy_instance();
private:
	SoundManager();

	void update_stream(MusicInstance& music);

	ALCdevice* al_device;
	ALCcontext* al_context;

	std::map<std::string, Sound> sound_map;
	std::map<std::string, Music> music_map;
	std::vector<GameObject*> objects;
	std::list<MusicInstance> active_music;

	static SoundManager* instance;
};