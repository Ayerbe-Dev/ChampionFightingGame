#pragma once
#include <iostream>
#include <vector>

struct MusicTrack {
	MusicTrack();
	MusicTrack(std::string data, char next_track);
	std::string data;
	char next_track;
};

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