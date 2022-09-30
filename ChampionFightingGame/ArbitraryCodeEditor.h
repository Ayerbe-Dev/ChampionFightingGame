#pragma once
#include <map>

class ArbitraryCodeEditor {
public:
	ArbitraryCodeEditor();

	void register_address();
	void inc_address();
	void dec_address();
	void inc_val();
	void dec_val();
	void apply();

	int address = 0;
	int val = 0;
	std::map<int, int> address_log;
};