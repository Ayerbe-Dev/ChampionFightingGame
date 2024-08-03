#include "Sound.h"
#include <fstream>
#include <vector>
#include "WindowManager.h"
#include "SoundManager.h"
#include "SaveManager.h"
#include "utils.h"

MusicTrack::MusicTrack() {
	data = "";
	next_track = -1;
}

MusicTrack::MusicTrack(std::string data, char next_track) {
	this->data = data;
	this->next_track = next_track;
}

Sound::Sound() {
	dir = "";
	buffer = 0;
	sample_rate = 0;
	format = AL_FORMAT_MONO8;
	track = "";
}

Sound::Sound(std::string dir) {
	this->dir = dir;
	std::uint8_t channels;
	std::uint8_t bits_per_sample;
	track = load_wav(dir, sample_rate, channels, bits_per_sample);
	if (!track.size()) {
		buffer = 0;
		return;
	}

	if (channels == 1 && bits_per_sample == 8) {
		format = AL_FORMAT_MONO8;
	}
	else if (channels == 1 && bits_per_sample == 16) {
		format = AL_FORMAT_MONO16;
	}
	else if (channels == 2 && bits_per_sample == 8) {
		format = AL_FORMAT_STEREO8;
	}
	else if (channels == 2 && bits_per_sample == 16) {
		format = AL_FORMAT_STEREO16;
	}
	else {
		std::cerr << "ERROR: unrecognised wave format: " << channels << " channels, " << bits_per_sample
			<< " bps\n";
		return;
	}

	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, track.c_str(), track.size(), sample_rate);
}

SoundInstance Sound::instantiate(float volume_mod) {
	return SoundInstance(buffer, volume_mod);
}

SoundInstance::SoundInstance() {
	sound_source = 0;
	volume_mod = 1.0f;
	al_source = 0;
	buffered = false;
	active = false;
}

SoundInstance::SoundInstance(unsigned int sound_source, float volume_mod) {
	this->sound_source = sound_source;
	this->volume_mod = volume_mod;
	al_source = 0;
	buffered = false;
	active = true;
}

void SoundInstance::start(glm::vec3* pos) {
	float volume_mul = SaveManager::get_instance()->get_game_setting("se_vol") / 100.0;
	alGenSources(1, &al_source);
	alSourcef(al_source, AL_PITCH, 1);
	if (pos != nullptr) {
		alSourcef(al_source, AL_GAIN, (1.0 + abs(WindowManager::get_instance()->camera.pos.z / 10.0) 
			+ volume_mod) * volume_mul);
		alSource3f(al_source, AL_POSITION, pos->x, pos->y, pos->z);
		alSourcei(al_source, AL_SOURCE_RELATIVE, AL_FALSE);
	}
	else {
		alSourcef(al_source, AL_GAIN, (1.0 + volume_mod) * volume_mul);
		alSource3f(al_source, AL_POSITION, 0.0, 0.0, 0.0);
		alSourcei(al_source, AL_SOURCE_RELATIVE, AL_TRUE);
	}
	alSource3f(al_source, AL_VELOCITY, 0.0, 0.0, 0.0);
	alSourcei(al_source, AL_LOOPING, AL_FALSE);
	alSourcei(al_source, AL_BUFFER, sound_source);
	alSourcePlay(al_source);
	buffered = true;
}

Music::Music() {
	dir = "";
	for (int i = 0; i < NUM_BUFFERS; i++) {
		buffers[i] = 0;
	}
	num_buffers = NUM_BUFFERS;
	sample_rate = 0;
	format = AL_FORMAT_MONO8;
}

Music::Music(std::string dir) {
	this->dir = dir;
	std::uint8_t channels;
	std::uint8_t bits_per_sample;
	num_buffers = NUM_BUFFERS;
	tracks = load_ayrbt(dir, sample_rate, channels, bits_per_sample);
	if (!tracks.size()) {
		for (int i = 0; i < NUM_BUFFERS; i++) {
			buffers[i] = 0;
		}
		return;
	}

	if (channels == 1 && bits_per_sample == 8) {
		format = AL_FORMAT_MONO8;
	}
	else if (channels == 1 && bits_per_sample == 16) {
		format = AL_FORMAT_MONO16;
	}
	else if (channels == 2 && bits_per_sample == 8) {
		format = AL_FORMAT_STEREO8;
	}
	else if (channels == 2 && bits_per_sample == 16) {
		format = AL_FORMAT_STEREO16;
	}
	else {
		std::cerr << "ERROR: unrecognised wave format: " << (int)channels << " channels, " << (int)bits_per_sample
			<< " bps\n";
		return;
	}

	alGenBuffers(NUM_BUFFERS, &buffers[0]);
	int total_track_size = 0;
	for (int i = 0, max = tracks.size(); i < max; i++) {
		total_track_size += tracks[i].data.size();
	}
	num_buffers = clamp(1, total_track_size / 4, 4);
	if (NUM_BUFFERS > num_buffers) {
		alDeleteBuffers(NUM_BUFFERS - num_buffers, &buffers[num_buffers]);
	}
	//Read update_stream to understand what this is doing, basically we're just populating buffers the same
	//way but starting from cursor pos 0
	reset_buffers();
}

void Music::reset_buffers() {
	for (int i = 0, cursor = 0, curr_track = 0; i < num_buffers; i++) {
		std::string data;
		data.resize(BUFFER_SIZE);
		std::memset(data.data(), 0, BUFFER_SIZE);
		std::size_t copied_size = 0;
		std::string track_data = tracks[curr_track].data;
		while (copied_size != BUFFER_SIZE) {
			std::size_t copy_size = BUFFER_SIZE - copied_size;
			if (cursor + copy_size > track_data.size()) {
				copy_size = track_data.size() - cursor;
			}
			std::memcpy(&data[copied_size], &track_data[cursor], copy_size);
			cursor += copy_size;

			if (copy_size != BUFFER_SIZE - copied_size) {
				curr_track = tracks[curr_track].next_track;
				track_data = tracks[curr_track].data;
				cursor = 0;
			}
			copied_size += copy_size;
		}
		alBufferData(buffers[i], format, data.c_str(), BUFFER_SIZE, sample_rate);
	}
}

MusicInstance* Music::instantiate(float volume_mod) {
	return SoundManager::get_instance()->play_music(dir, volume_mod);
}

MusicInstance::MusicInstance() {
	dir = "";
	volume_mod = 1.0;
	cursor = 0;
	curr_track = 0;
	source = 0;
}

bool load_wav_file_header(std::ifstream& file,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample,
	int& size)
{
	char buffer[4];
	if (!file.is_open()) {
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read RIFF\n";
		return false;
	}
	if (std::strncmp(buffer, "RIFF", 4) != 0) {
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)\n";
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read size of file\n";
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read WAVE\n";
		return false;
	}
	if (std::strncmp(buffer, "WAVE", 4) != 0) {
		std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)\n";
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read fmt/0\n";
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read the 16\n";
		return false;
	}

	if (!file.read(buffer, 2)) {
		std::cerr << "ERROR: could not read PCM\n";
		return false;
	}

	if (!file.read(buffer, 2)) {
		std::cerr << "ERROR: could not read number of channels\n";
		return false;
	}
	channels = convert_to_int(buffer, 2);

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read sample rate\n";
		return false;
	}
	sample_rate = convert_to_int(buffer, 4);

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8\n";
		return false;
	}

	if (!file.read(buffer, 2)) {
		std::cerr << "ERROR: could not read this badly doc'd section of the header\n";
		return false;
	}

	if (!file.read(buffer, 2)) {
		std::cerr << "ERROR: could not read bits per sample\n";
		return false;
	}
	bits_per_sample = convert_to_int(buffer, 2);

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read what should be either data or LIST chunk header\n";
		return false;
	}
	if (std::strncmp(buffer, "LIST", 4) == 0) {
		if (!file.read(buffer, 4)) {
			std::cerr << "ERROR: could not read size of LIST data\n";
			return false;
		}
		if (!file.ignore(convert_to_int(buffer, 4))) {
			std::cerr << "ERROR: could not pretend to read through the LIST data\n";
			return false;
		}
		if (!file.read(buffer, 4)) {
			std::cerr << "ERROR: could not read data chunk header\n";
			return false;
		}
	}
	if (std::strncmp(buffer, "data", 4) != 0) {
		std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag), instead has "
			<< buffer << "\n";
		return false;
	}

	if (!file.read(buffer, 4)) {
		std::cerr << "ERROR: could not read data size\n";
		return false;
	}
	size = convert_to_int(buffer, 4);

	if (file.eof()) {
		std::cerr << "ERROR: reached EOF on the file\n";
		return false;
	}
	if (file.fail()) {
		std::cerr << "ERROR: fail state set on the file" << std::endl;
		return false;
	}

	return true;
}

std::string load_wav(const std::string& filename,
	std::int32_t& sample_rate,
	std::uint8_t& channels,
	std::uint8_t& bits_per_sample)
{
	int size = 0;
	std::ifstream in(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "ERROR: Could not open \"" << filename << "\"\n";
		return "";
	}
	if (!load_wav_file_header(in, sample_rate, channels, bits_per_sample, size)) {
		std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"\n";
		return "";
	}

	std::string data;
	data.resize(size);

	in.read(data.data(), size);
	return data;
}