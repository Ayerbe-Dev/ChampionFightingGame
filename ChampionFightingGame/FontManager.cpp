#include "FontManager.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "WindowConstants.h"

//TODO: This entire class can be condensed into the ResourceManager so unload_unused handles unused 
//faces automatically

FontManager::FontManager() {
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
	}
}

void FontManager::load_face(std::string name) {
	if (loaded_faces.contains(name)) {
		return;
	}
	FT_Face face;
	std::string dir = "resource/fonts/" + name + ".ttf";
	if (FT_New_Face(ft, dir.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font at " << dir << "\n";
		return;
	}

	loaded_faces[name] = face;
	face_names.push_back(name);
}

void FontManager::unload_face(std::string name) {
	if (!loaded_faces.contains(name)) {
		return;
	}
	FT_Done_Face(loaded_faces[name]);
	loaded_faces.erase(name);
}

Font FontManager::load_font(std::string name, int size) {
	if (!loaded_faces.contains(name)) {
		std::cout << "Repeating Load Font\n";
		GameManager::get_instance()->add_crash_log("Face for font " + name + " not loaded!");
		return Font();
	}
	FT_Face& face = loaded_faces[name];
	FT_Set_Pixel_Sizes(face, 0, size);

	Font ret;
	float max_bearing = 0.0f;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			GameManager::get_instance()->add_crash_log("Failed to load char " + c);
			continue;
		}
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		TexChar tex_char;
		tex_char.texture = texture;
		tex_char.size = glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		tex_char.bearing = glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		tex_char.advance = face->glyph->advance.x;
		ret.base_y_offset = std::max(tex_char.size.y - tex_char.bearing.y, ret.base_y_offset);
		max_bearing = std::max(tex_char.bearing.y, max_bearing);
		ret.char_map.insert(std::pair<char, TexChar>(c, tex_char));
	}
	ret.base_height = max_bearing + ret.base_y_offset;
	
	return ret;
}

FontManager* FontManager::instance = nullptr;
FontManager* FontManager::get_instance() {
	if (instance == nullptr) {
		instance = new FontManager;
	}
	return instance;
}

void FontManager::destroy_instance() {
	for (int i = 0, max = face_names.size(); i < max; i++) {
		unload_face(face_names[i]);
	}
	FT_Done_FreeType(ft);
	if (instance != nullptr) {
		delete instance;
	}
}