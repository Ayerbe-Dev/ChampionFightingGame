#include "Sound.h"

SoundInfo::SoundInfo() {
	sound_kind = SOUND_KIND_MAX;
}

SoundInfo::SoundInfo(std::string name, std::string dir, int sound_kind, int sound_type, int volume) {
	this->name = name;
	switch (sound_kind) {
		case(SOUND_KIND_MUSIC): {
			dir = "resource/sound/bgm/" + dir + "/" + name;
		} break;
		case (SOUND_KIND_SE): {
			dir = "resource/sound/se/" + dir + "/" + name;
		} break;
		case (SOUND_KIND_VC):
		default: {
			dir = "resource/sound/vc/" + dir + "/" + name;
		} break;
	}
	this->dir = dir + ".wav";
	if (sound_type == SOUND_TYPE_LOOP) {
		this->loop_dir = dir + "_loop.wav";
	}
	this->volume = volume;
	this->sound_kind = sound_kind;
	this->sound_type = sound_type;
}

Sound::Sound() {
	info = nullptr;
	data = nullptr;
	loop_data = nullptr;
	duration = -1;
	loop_duration = -1;
}

Sound::Sound(SoundInfo *info) {
	init(info);
}

void Sound::init(SoundInfo *info) {
	this->info = info;
}

SoundInstance Sound::instantiate(int volume) {
	if (volume == -1) {
		volume = info->volume;
	}
	SoundInstance ret(this, volume);
	return ret;
}

SoundInstance::SoundInstance() {
	active = true;
	looped = false;
	sound = nullptr;
	volume = -1;
	pos = 0;
}

SoundInstance::SoundInstance(Sound* sound, int volume) {
	active = true;
	looped = false;
	this->sound = sound;
	this->volume = volume;
	pos = 0;
}