#include "Prmlst.h"
#include "Param.h"

enum {
	PARAM_TYPE_INT,
	PARAM_TYPE_FLOAT,
	PARAM_TYPE_STRING,
	PARAM_TYPE_BOOL,
	PARAM_TYPE_PARAMLIST,

	PARAM_TYPE_MAX,
};

void parse_param_entry(std::ifstream& stream, std::string& name, int& type, std::any& value) {
	name.clear();
	char input_char;
	stream.read(&input_char, 1); //Using this to represent the size of our name string
	for (int i = 0, max = (int)input_char - 3; i < max; i += 4) {
		char chars[3];
		stream.read(chars, 3);
		unsigned uchars[3];
		for (int i = 0; i < 3; i++) {
			uchars[i] = (unsigned)(chars[i]) % 256;
		}

		name.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
		name.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
		name.push_back(get_prmlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
		name.push_back(get_prmlst_char((uchars[2] % 64)));
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
				name.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
				name.push_back(get_prmlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
			} break;
			case(2): {
				char chars[2];
				stream.read(chars, 2);
				unsigned uchars[2];
				for (int i = 0; i < 2; i++) {
					uchars[i] = (unsigned)(chars[i]) % 256;
				}
				name.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
				name.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
			} break;
			default: {
				char c;
				stream.read(&c, 1);
				name.push_back(get_prmlst_char(((unsigned)(c % 256) >> 2) % 64));
			} break;
		}
	}
	stream.read(&input_char, 1);
	type = (((unsigned int)input_char) & 0xC0) >> 6;
	if (type == PARAM_TYPE_INT) {
		//If bit 7 is turned on for an int, then the int isn't actually an int. 
		//You've been bamboozled.
		if (((unsigned)input_char) & 0x2) {
			type = PARAM_TYPE_PARAMLIST;
		}
	}
	switch (type) {
		case (PARAM_TYPE_INT):
		default: { //For ints, we may not use all the bytes, so we check how many are necessary
			unsigned int num_bytes = ((((unsigned int)input_char) & 0x30) >> 4) + 1;
			switch (num_bytes) {
				case (1): {
					stream.read(&input_char, 1);
					int value_i = ((unsigned int)input_char) & 0xFF;
					value = value_i;
				} break;
				case (2): {
					char chars[2];
					stream.read(chars, 2);
					unsigned int result = 0;
					for (size_t i = 0; i < 2; i++) {
						result += (((unsigned int)chars[i]) & 0xFF) << (8 * (1 - i));
					}
					int value_i = (int)result;
					value = value_i;
				} break;
				case (3): {
					char chars[3];
					stream.read(chars, 3);
					unsigned int result = 0;
					for (size_t i = 0; i < 3; i++) {
						result += (((unsigned int)chars[i]) & 0xFF) << (8 * (2 - i));
					}
					int value_i = (int)result;
					value = value_i;
				} break;
				case (4):
				default: {
					char chars[4];
					stream.read(chars, 4);
					unsigned int result = 0;
					for (size_t i = 0; i < 4; i++) {
						result += (((unsigned int)chars[i]) & 0xFF) << (8 * (3 - i));
					}
					int value_i = (int)result;
					value = value_i;
				} break;
			}
		} break;
		case (PARAM_TYPE_FLOAT): {
			char chars[4];
			stream.read(chars, 4);
			unsigned int result = 0;
			for (size_t i = 0; i < 4; i++) {
				result += (((unsigned int)chars[i]) & 0xFF) << (8 * (3 - i));
			}
			float value_f;
			memcpy(&value_f, &result, sizeof(unsigned int));
			value = value_f;
		} break;
		case (PARAM_TYPE_STRING): { //And for strings, it gets weird
			std::string value_s;
			unsigned int size = ((((unsigned int)input_char) & 0x3C) >> 2) & 0xF;
			bool uncompressed = input_char & 0x2;
			if (input_char & 0x1) {
				size <<= 8;
				stream.read(&input_char, 1);
				size += ((unsigned int)input_char) & 0xFF;
			}
			if (uncompressed) {
				value_s.resize(size);
				stream.read(value_s.data(), size);
			}
			else {
				for (int i = 0, max = (int)size - 3; i < max; i += 4) {
					char chars[3];
					stream.read(chars, 3);
					unsigned uchars[3];
					for (int i = 0; i < 3; i++) {
						uchars[i] = (unsigned)(chars[i]) % 256;
					}

					value_s.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
					value_s.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
					value_s.push_back(get_prmlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
					value_s.push_back(get_prmlst_char((uchars[2] % 64)));
				}
				size_t rem = size % 4;
				if (rem != 0) {
					switch (rem) {
						case(3): {
							char chars[3];
							stream.read(chars, 3);
							unsigned uchars[3];
							for (int i = 0; i < 3; i++) {
								uchars[i] = (unsigned)(chars[i]) % 256;
							}
							value_s.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
							value_s.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
							value_s.push_back(get_prmlst_char(((uchars[1] << 2) + (uchars[2] >> 6)) % 64));
						} break;
						case(2): {
							char chars[2];
							stream.read(chars, 2);
							unsigned uchars[2];
							for (int i = 0; i < 2; i++) {
								uchars[i] = (unsigned)(chars[i]) % 256;
							}
							value_s.push_back(get_prmlst_char((uchars[0] >> 2) % 64));
							value_s.push_back(get_prmlst_char(((uchars[0] << 4) + (uchars[1] >> 4)) % 64));
						} break;
						default: {
							char c;
							stream.read(&c, 1);
							value_s.push_back(get_prmlst_char(((unsigned)(c % 256) >> 2) % 64));
						} break;
					}
				}
			}
			value = value_s;
		} break;
		case (PARAM_TYPE_BOOL): { //For bools, we don't even need to read in another byte
			bool value_b = input_char & 0x20;
			value = value_b;
		} break;
		case (PARAM_TYPE_PARAMLIST): {
			unsigned size = ((((unsigned)input_char) & 0x3C) >> 2) & 0xF;
			if (input_char & 0x1) {
				size <<= 8;
				stream.read(&input_char, 1);
				size += ((unsigned)input_char) & 0xFF;
			}
			ParamTable value_p;
			for (size_t i = 0; i < size; i++) {
				Param param;
				parse_param_entry(stream, param.name, param.type, param.value);
				if (!stream.eof() || i == size - 1) {
					value_p.add_param(param, i);
				}
			}

			value = value_p;
		} break;

	}
	stream.peek(); //Sets eof if this is the last entry
}

char get_prmlst_char(char key) {
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