#include "ArbitraryCodeEditor.h"

ArbitraryCodeEditor::ArbitraryCodeEditor() {

}

void ArbitraryCodeEditor::register_address() {
	if (address >= (int)this
		&& address < (int)this + sizeof(ArbitraryCodeEditor)) {
		return;
	}
	address_log[address] = val;
}

void ArbitraryCodeEditor::inc_address() {
	address++;
}

void ArbitraryCodeEditor::dec_address() {
	address--;
}

void ArbitraryCodeEditor::inc_val() {
	val++;
}

void ArbitraryCodeEditor::dec_val() {
	val--;
}

void ArbitraryCodeEditor::apply() {
	for (const auto& addresses : address_log) {
		*(int*)addresses.first = addresses.second;
	}
	address_log.clear();
}