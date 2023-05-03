#include "ReadBytes.h"

bool read_bytes_to_string(std::ifstream* stream, std::string* string, unsigned num_bytes) {
	string->resize(num_bytes);
	stream->read(string->data(), num_bytes);
	if (stream->fail()) {
		std::cout << "Failed to read " << num_bytes << " bytes, ended up with string \'" << *string << "\'\n";
		return false;
	}
	return true;
}