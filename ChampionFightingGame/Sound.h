#pragma once
#include <iostream>
#include <vector>
#include "OpenAL/alc.h"
#include "OpenAL/al.h"
#include "Ayrbt.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

struct Sound {
	Sound();
	Sound(std::string dir, float volume_mod);

	unsigned int buffer;
	float volume_mod;
	std::int32_t sample_rate;
	ALenum format;
	std::string track;
};

struct Music {
	Music();
	Music(std::string dir, float volume_mod);

	void reset_buffers();

	unsigned int buffers[NUM_BUFFERS];
	unsigned int num_buffers;
	float volume_mod;
	std::int32_t sample_rate;
	ALenum format;
	std::vector<MusicTrack> tracks;
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