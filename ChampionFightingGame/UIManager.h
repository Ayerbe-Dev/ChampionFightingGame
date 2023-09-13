#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "TextUI.h"

class UIManager {
public:
	UIManager(UIManager& other) = delete;
	void operator=(const UIManager& other) = delete;

	int get_num_text_ui() const;
	TextUI& get_text_ui(std::size_t id);
	TextUI& get_text_ui(std::string name);
	void add_text_ui(std::string name); //Other construction args go here

	static UIManager* get_instance();
	void destroy_instance();
private:
	UIManager();
	static UIManager* instance;

	std::vector<TextUI> text_ui;
	std::map<std::string, std::size_t> text_ui_names;
};