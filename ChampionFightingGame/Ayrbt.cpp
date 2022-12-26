#include "Ayrbt.h"
#include <fstream>
#include "utils.h"

//Note: As mentioned in Sound.h, ayrbt is not a real file format at the moment, and as such I don't know
//what the header looks like. It's not going to stop me from writing the rest of the file loading code,
//but even I have to draw the line somewhere

bool load_ayrbt_file_header(std::ifstream& file,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample
) {
	std::string buffer;
	buffer.resize(9);
	file.read(buffer.data(), 9);
	if (buffer != "AYRBT 1.0") {
		std::cerr << "ERROR: Given file is not Ayrbt! (Does not begin with AYRBT 1.0)\n";
		return false;
	}

	if (!file.read(buffer.data(), 1)) {
		std::cerr << "Failed to read bytes in sample rate!\n";
		return false;
	}
	int bytes_for_sample_rate = (int)buffer[0];

	if (!file.read(buffer.data(), bytes_for_sample_rate)) {
		std::cerr << "Failed to read sample rate!\n";
		return false;
	}
	sample_rate = convert_to_int(buffer.data(), bytes_for_sample_rate);

	if (!file.read(buffer.data(), 1)) {
		std::cerr << "Failed to read num channels!\n";
		return false;
	}
	channels = convert_to_int(buffer.data(), 1);

	if (!file.read(buffer.data(), 1)) {
		std::cerr << "Failed to read bits per sample!\n";
		return false;
	}
	bits_per_sample = convert_to_int(buffer.data(), 1);

	return true;
}

std::vector<MusicTrack> load_ayrbt(const std::string& filename,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample
) {
	std::vector<MusicTrack> ret;
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "ERROR: Could not open \"" << filename << "\"\n";
		return ret;
	}
	if (!load_ayrbt_file_header(in, sample_rate, channels, bits_per_sample)) {
		std::cerr << "ERROR: Could not load ayrbt header of \"" << filename << "\"\n";
		return ret;
	}

	std::string buffer;
	for (char next_track, bytes_for_data; in.read(&next_track, 1) && in.read(&bytes_for_data, 1); ret.emplace_back(buffer, next_track)) {
		buffer.resize(bytes_for_data);
		in.read(buffer.data(), bytes_for_data);
		unsigned int data_size = convert_to_int(buffer.data(), (size_t)bytes_for_data);
		buffer.resize(data_size);
		in.read(buffer.data(), data_size);
	}

	return ret;
}