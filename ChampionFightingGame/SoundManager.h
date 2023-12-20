#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <array>
#include <map>
#include "Sound.h"

struct SoundResource {
	SoundResource();
	Sound sound;
	int user_count;
};

struct MusicResource {
	MusicResource();
	Music music;
	int user_count;
};

class SoundPlayer;

class SoundManager {
public:
	SoundManager(SoundManager& other) = delete;
	void operator=(const SoundManager& other) = delete;

	void register_sound_player(SoundPlayer* sound_player);
	void unregister_sound_player(SoundPlayer* sound_player);

	void clear_sound_players();

	void process_sounds();

	MusicInstance* play_music(std::string dir, float volume_mod);
	void pause_music(std::string dir);
	void pause_music_all();
	void resume_music(std::string dir);
	void resume_music_all();
	void stop_music(std::string dir);
	void stop_music_all();
;
	void pause_all_reserved_sounds();
	void pause_all_sounds();
	void resume_all_reserved_sounds();
	void resume_all_sounds();


	Sound& get_sound(std::string dir);
	Music& get_music(std::string dir);
	void load_sound(std::string dir);
	void load_music(std::string dir);
	void unuse_sound(std::string dir);
	void unuse_music(std::string dir);
	void unload_sound(std::string dir, bool strict = true);
	void unload_music(std::string dir, bool strict = true);
	void unload_all_sounds();
	void unload_all_music();

	void unload_unused();

	static SoundManager* get_instance();
	void destroy_instance();
private:
	SoundManager();

	void update_stream(MusicInstance& music);

	ALCdevice* al_device;
	ALCcontext* al_context;

	std::map<std::string, SoundResource> sound_map;
	std::map<std::string, MusicResource> music_map;
	std::list<SoundPlayer*> sound_players;
	std::list<MusicInstance> active_music;

	static SoundManager* instance;
};