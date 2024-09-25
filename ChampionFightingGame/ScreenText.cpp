#include "ScreenText.h"

ScreenText::ScreenText() {
	text = "";
	font = nullptr;
}

ScreenText& ScreenText::init() {
	return *this;
}

ScreenText& ScreenText::update_text(std::string text) {
	return *this;
}

void ScreenText::render() {

}