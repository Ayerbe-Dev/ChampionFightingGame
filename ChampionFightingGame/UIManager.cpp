#include "UIManager.h"

UIManager::UIManager() {

}

int UIManager::get_num_text_ui() const {
	return text_ui.size();
}

TextUI& UIManager::get_text_ui(std::size_t id) {
	return text_ui[id];
}
	
TextUI& UIManager::get_text_ui(std::string name) {
	if (!text_ui_names.contains(name)) {
		text_ui_names[name] = text_ui.size();
		text_ui.push_back(TextUI());
	}
	return text_ui[text_ui_names[name]];
}
	
void UIManager::add_text_ui(std::string name) {
	if (name != "") {
		text_ui_names[name] = text_ui.size();
	}
	text_ui.push_back(TextUI());
}

UIManager* UIManager::instance = nullptr;
UIManager* UIManager::get_instance() {
	if (instance == nullptr) {
		instance = new UIManager;
	}
	return instance;
}

void UIManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}