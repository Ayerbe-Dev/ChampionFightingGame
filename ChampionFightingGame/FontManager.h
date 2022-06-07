#pragma once
#include <freetype/ft2build.h>
#include <freetype/freetype.h>
#include <vector>
#include <map>
#include "Font.h"

class FontManager {
public:
	FontManager(FontManager& other) = delete;
	void operator=(const FontManager& other) = delete;

	void load_face(std::string name);
	void unload_face(std::string name);

	Font load_font(std::string name, int size);

	static FontManager* get_instance();
	void destroy_instance();
private:
	FT_Library ft;
	std::map<std::string, FT_Face> loaded_faces;
	std::vector<std::string> face_names;

	FontManager();
	static FontManager* instance;
};