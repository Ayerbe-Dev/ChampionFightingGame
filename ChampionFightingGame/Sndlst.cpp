#include "Sndlst.h"

void parse_sndlst_entry(std::ifstream& stream, std::string& name, std::string& filename, float& volume_mod) {
	name.clear();
	filename.clear();
	char input_char;
	stream.read(&input_char, 1); //Using this to represent the size of our name string
	if (stream.eof()) {
		return;
	}
	for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
		char chars[3];
		stream.read(chars, 3);
		unsigned uchars[3];
		for (int i = 0; i < 3; i++) {
			uchars[i] = (unsigned)(chars[i]) % 256;
		}

		name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
		name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
		name.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
		name.push_back(get_sndlst_char((uchars[2] % 64)));
	}
	size_t rem = input_char % 4;
	if (rem != 0) {
		switch (rem) {
			case(3): {
				char chars[3];
				stream.read(chars, 3);
				unsigned uchars[3];
				for (int i = 0; i < 3; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				name.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			} break;
			case(2): {
				char chars[2];
				stream.read(chars, 2);
				unsigned uchars[2];
				for (int i = 0; i < 2; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			} break;
			default: {
				char c;
				stream.read(&c, 1);
				name.push_back(get_sndlst_char(((unsigned)(c % 256) >> 2) % 64));
			} break;
		}
	}
	stream.read(&input_char, 1); //Using this to represent either the size of our filename string or 
	//our indicator that it's just the name again
	if (input_char == '\0') {
		filename = name;
	}
	else {
		for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
			char chars[3];
			stream.read(chars, 3);
			unsigned uchars[3];
			for (int i = 0; i < 3; i++) {
				uchars[i] = (unsigned)(chars[i]) % 256;
			}

			filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
			filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			filename.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			filename.push_back(get_sndlst_char((uchars[2] % 64)));
		}
		size_t rem = input_char % 4;
		if (rem != 0) {
			switch (rem) {
				case(3): {
					char chars[3];
					stream.read(chars, 3);
					unsigned uchars[3];
					for (int i = 0; i < 3; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
					filename.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
				} break;
				case(2): {
					char chars[2];
					stream.read(chars, 2);
					unsigned uchars[2];
					for (int i = 0; i < 2; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				} break;
				default: {
					char c;
					stream.read(&c, 1);
					filename.push_back(get_sndlst_char(((unsigned)(c % 256) >> 2) % 64));
				} break;
			}
		}
	}
	filename += ".wav";
	char chars[4];
	stream.read(chars, 4);
	unsigned int result = 0;
	for (size_t i = 0; i < 4; i++) {
		result += (((unsigned int)chars[i]) & 0xFF) << (8 * (3 - i));
	}
	float value_f;
	memcpy(&value_f, &result, sizeof(unsigned int));
	volume_mod = value_f;
	stream.peek();
}

void parse_sndlst_entry(std::ifstream& stream, std::string& name, std::string& filename) {
	name.clear();
	filename.clear();
	char input_char;
	stream.read(&input_char, 1); //Using this to represent the size of our name string
	if (stream.eof()) {
		return;
	}
	for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
		char chars[3];
		stream.read(chars, 3);
		unsigned uchars[3];
		for (int i = 0; i < 3; i++) {
			uchars[i] = (unsigned)(chars[i]) % 256;
		}

		name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
		name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
		name.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
		name.push_back(get_sndlst_char((uchars[2] % 64)));
	}
	size_t rem = input_char % 4;
	if (rem != 0) {
		switch (rem) {
			case(3): {
				char chars[3];
				stream.read(chars, 3);
				unsigned uchars[3];
				for (int i = 0; i < 3; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				name.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			} break;
			case(2): {
				char chars[2];
				stream.read(chars, 2);
				unsigned uchars[2];
				for (int i = 0; i < 2; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			} break;
			default: {
				char c;
				stream.read(&c, 1);
				name.push_back(get_sndlst_char(((unsigned)(c % 256) >> 2) % 64));
			} break;
		}
	}
	stream.read(&input_char, 1); //Using this to represent either the size of our filename string or 
	//our indicator that it's just the name again
	if (input_char == '\0') {
		filename = name;
	}
	else {
		for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
			char chars[3];
			stream.read(chars, 3);
			unsigned uchars[3];
			for (int i = 0; i < 3; i++) {
				uchars[i] = (unsigned)(chars[i]) % 256;
			}

			filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
			filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			filename.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			filename.push_back(get_sndlst_char((uchars[2] % 64)));
		}
		size_t rem = input_char % 4;
		if (rem != 0) {
			switch (rem) {
				case(3): {
					char chars[3];
					stream.read(chars, 3);
					unsigned uchars[3];
					for (int i = 0; i < 3; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
					filename.push_back(get_sndlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
				} break;
				case(2): {
					char chars[2];
					stream.read(chars, 2);
					unsigned uchars[2];
					for (int i = 0; i < 2; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_sndlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_sndlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				} break;
				default: {
					char c;
					stream.read(&c, 1);
					filename.push_back(get_sndlst_char(((unsigned)(c % 256) >> 2) % 64));
				} break;
			}
		}
	}
	filename += ".wav";
	char chars[4];
	stream.read(chars, 4);
	stream.peek();
}

char get_sndlst_char(char key) {
	if (key == 0) { //null
		return key;
	}
	else if (key >= 1 && key < 27) { //A-Z
		return key + 64;
	}
	else if (key >= 27 && key < 53) { //a-z
		return key + 70;
	}
	else if (key >= 53 && key < 63) { //0-9
		return key - 7;
	}
	else if (key == 63) { //_
		return 95;
	}
	else {
		return (char)255;
	}
}