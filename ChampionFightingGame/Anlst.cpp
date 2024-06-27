#include "Anlst.h"

#define ANIM_FLAG_MOVE 1;
#define ANIM_FLAG_NO_HITLAG_INTERP 2;
#define ANIM_FLAG_UNUSED 4;

void parse_anlst_entry(std::ifstream& stream, std::string& name, std::string& filename,
	int& end_frame, bool& flag_move, bool& flag_no_hitlag_interp, bool& flag_unused) {
	name.clear();
	filename.clear();
	char input_char;
	stream.read(&input_char, 1); //Using this to represent the size of our name string
	if (stream.eof()) {
		return;
	}
	stream.tellg();
	for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
		char chars[3];
		stream.read(chars, 3);
		unsigned uchars[3];
		for (int i = 0; i < 3; i++) {
			uchars[i] = (unsigned)(chars[i]) % 256;
		}

		name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
		name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
		name.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
		name.push_back(get_anlst_char((uchars[2] % 64)));
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
				name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				name.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			} break;
			case(2): {
				char chars[2];
				stream.read(chars, 2);
				unsigned uchars[2];
				for (int i = 0; i < 2; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			} break;
			default: {
				char c;
				stream.read(&c, 1);
				name.push_back(get_anlst_char(((unsigned)(c % 256) >> 2) % 64));
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

			filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
			filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			filename.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			filename.push_back(get_anlst_char((uchars[2] % 64)));
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
					filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
					filename.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
				} break;
				case(2): {
					char chars[2];
					stream.read(chars, 2);
					unsigned uchars[2];
					for (int i = 0; i < 2; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				} break;
				default: {
					char c;
					stream.read(&c, 1);
					filename.push_back(get_anlst_char(((unsigned)(c % 256) >> 2) % 64));
				} break;
			}
		}
	}
	if (filename != "none") {
		filename += ".fbx";
	}

	stream.read(&input_char, 1);
	end_frame = (((unsigned short)input_char & 0xFF) << 5) & 0x1FFF;
	stream.read(&input_char, 1);
	end_frame += (((unsigned short)input_char & 0xFF) >> 3) & 0x1F;
	if (end_frame == 8191) {
		end_frame = -1;
	}
	flag_move = input_char & ANIM_FLAG_MOVE;
	flag_no_hitlag_interp = input_char & ANIM_FLAG_NO_HITLAG_INTERP;
	flag_unused = input_char & ANIM_FLAG_UNUSED;
	stream.peek(); //Sets eof if this is the last entry
}

void parse_anlst_entry(std::ifstream& stream, std::string& name, std::string& filename) {
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

		name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
		name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
		name.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
		name.push_back(get_anlst_char((uchars[2] % 64)));
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
				name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				name.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			} break;
			case(2): {
				char chars[2];
				stream.read(chars, 2);
				unsigned uchars[2];
				for (int i = 0; i < 2; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_anlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			} break;
			default: {
				char c;
				stream.read(&c, 1);
				name.push_back(get_anlst_char(((unsigned)(c % 256) >> 2) % 64));
			} break;
		}
	}
	stream.read(&input_char, 1);
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

			filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
			filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			filename.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			filename.push_back(get_anlst_char((uchars[2] % 64)));
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
					filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
					filename.push_back(get_anlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
				} break;
				case(2): {
					char chars[2];
					stream.read(chars, 2);
					unsigned uchars[2];
					for (int i = 0; i < 2; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}
					filename.push_back(get_anlst_char((uchars[0] >> 2) % 64));
					filename.push_back(get_anlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				} break;
				default: {
					char c;
					stream.read(&c, 1);
					filename.push_back(get_anlst_char(((unsigned)(c % 256) >> 2) % 64));
				} break;
			}
		}
	}
	if (filename != "none") {
		filename += ".fbx";
	}

	stream.read(&input_char, 1);
	stream.read(&input_char, 1);
	stream.peek(); //Sets eof if this is the last entry
}

char get_anlst_char(char key) {
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
		return key - 5;
	}
	else if (key == 63) { //_
		return 95;
	}
	else {
		return (char)255;
	}
}