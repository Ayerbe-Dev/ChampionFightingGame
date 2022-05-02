#pragma once
#include <iostream>
#include "SoundConstants.h"
#include "CharaKind.h"
#include <SDL/SDL_audio.h>

class SoundInfo {
public:
	SoundInfo();
	SoundInfo(std::string name, int sound_kind, int chara_kind = CHARA_KIND_MAX, int volume = 32, int sound_type = SOUND_TYPE_NORMAL);

	std::string name;
	std::string dir;
	std::string loop_dir;
	int sound_kind;
	int sound_type;
	int volume;
	bool looped;
	bool active;
};

class Sound {
public:
	Sound();
	Uint8* data;
	Uint8* loop_data;
	unsigned int dpos;
	unsigned int dlen;
	unsigned int loop_dlen;
	SoundInfo sound;
};