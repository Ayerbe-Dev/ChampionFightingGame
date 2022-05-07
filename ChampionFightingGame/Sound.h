#pragma once
#include <iostream>
#include "SoundConstants.h"
#include "CharaKind.h"
#include <SDL/SDL_audio.h>

class SoundInstance;

class SoundInfo {
public:
	SoundInfo();
	SoundInfo(std::string name, std::string dir, int sound_kind, int sound_type, int volume);

	std::string name;
	std::string dir;
	std::string loop_dir;
	int sound_kind;
	int sound_type;
	int volume;
};

class Sound {
public:
	Sound();
	Sound(SoundInfo *info);

	void init(SoundInfo *info);
	SoundInstance instantiate(int volume);

	Uint8* data;
	Uint8* loop_data;
	unsigned int duration;
	unsigned int loop_duration;
	SoundInfo *info;
};

class SoundInstance {
public:
	SoundInstance();
	SoundInstance(Sound* sound, int volume);

	unsigned int pos;
	Sound *sound;
	bool active;
	bool looped;
	int volume;
};