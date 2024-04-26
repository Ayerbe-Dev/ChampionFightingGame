#pragma once
#include "SoundManager.h"
#include <glm/glm.hpp>

class SoundPlayer {
public:
	SoundPlayer();
	~SoundPlayer();

	void init(glm::vec3 *object_pos);
	bool is_sound_end();

	void process_sound();
	void play_sound(std::string name, float volume_mod);
	void pause_sound(std::string name);
	void pause_sound_all();
	void resume_sound(std::string name);
	void resume_sound_all();
	void stop_sound(std::string name);
	void stop_sound_all();

	void play_reserved_sound(std::string name, float volume_mod);
	void pause_reserved_sound();
	void resume_reserved_sound();
	void stop_reserved_sound();

	void load_sound_list(std::string list, std::string dir);
	void load_sound(std::string name, std::string dir);
	void unload_sound(std::string name);
	void unload_all_sounds();
private:
	SoundManager* sound_manager;
	std::map<std::string, Sound> sound_data;
	std::list<SoundInstance> sound_instances;
	SoundInstance reserved_sound;
	bool sound_end;

	glm::vec3 *object_pos;
};