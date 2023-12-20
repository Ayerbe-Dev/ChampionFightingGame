#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "OpenAL/alc.h"
#include "OpenAL/al.h"
#include "Ayrbt.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

struct SoundInstance {
	SoundInstance();
	SoundInstance(unsigned int sound_source, float volume_mod);

	void start(glm::vec3* pos);

	unsigned int sound_source;
	unsigned int al_source;
	float volume_mod;

	bool buffered;
	bool active;
};

struct Sound {
	Sound();
	Sound(std::string dir);

	SoundInstance instantiate(float volume_mod);

	std::string dir;
	unsigned int buffer;
	std::int32_t sample_rate;
	ALenum format;
	std::string track;
};

struct MusicInstance {
	MusicInstance();

	std::string dir;
	float volume_mod;
	unsigned int cursor;
	char curr_track;
	unsigned int source;
};

struct Music {
	Music();
	Music(std::string dir);

	MusicInstance* instantiate(float volume_mod);

	void reset_buffers();

	std::string dir;
	unsigned int buffers[NUM_BUFFERS];
	unsigned int num_buffers;
	std::int32_t sample_rate;
	ALenum format;
	std::vector<MusicTrack> tracks;

	unsigned int cursor;
	char curr_track;
	unsigned int source;
};

bool load_wav_file_header(std::ifstream& file,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample,
	int& size
);
std::string load_wav(const std::string& filename,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample
);