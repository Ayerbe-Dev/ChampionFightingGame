#pragma once
#include <iostream>
#include <vector>
#include "OpenAL/alc.h"
#include "OpenAL/al.h"

#define NUM_BUFFERS 4
#define BUFFER_SIZE 65536

struct MusicTrack {
	MusicTrack();
	MusicTrack(std::string data, char next_track);
	std::string data;
	char next_track;
};

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

std::int32_t convert_to_int(char* buffer, std::size_t len);
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

//Note: ayrbt stands for "Ayerbe Track". It is a proprietary audio format designed to support multiple
//tracks per file with specified behavior for what to do when the end of a track is reached. Unfortunately,
//it has the minor downside of "not actually existing", but I'm still going to write code to support my 
//silly little hypothetical audio format.

bool load_ayrbt_file_header(std::ifstream& file,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample
);
std::vector<MusicTrack> load_ayrbt(const std::string& filename,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample
);