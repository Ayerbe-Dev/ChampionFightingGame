#pragma once
#include "ScreenTexture.h"
#include "ScreenText.h"
#include "WorldTexture.h"
#include "WorldText.h"
#include "SoundPlayer.h"
#include <any>
#include <vector>
#include <map>
#include <functional>

const int ELEM_TYPE_ELEM = 0;
const int ELEM_TYPE_SCRN_TEX = 1;
const int ELEM_TYPE_SCRN_TXT = 2;
const int ELEM_TYPE_WRLD_TEX = 3;
const int ELEM_TYPE_WRLD_TXT = 4;

class Scene;
class SceneElement {
public:
	SceneElement();
	SceneElement(std::vector<std::pair<std::string, std::any>> elements);

	SceneElement(SceneElement& other);
	SceneElement(const SceneElement& other);
	SceneElement(SceneElement&& other) noexcept;
	SceneElement& operator=(SceneElement& other);
	SceneElement& operator=(const SceneElement& other);
	SceneElement& operator=(SceneElement&& other) noexcept;
	~SceneElement();

	std::string get_name() const;

	SceneElement& add_element(std::string name, std::any element);
	SceneElement& add_elements(std::vector<std::pair<std::string, std::any>> elements);

	SceneElement& get_child(std::string name);
	SceneElement& get_child(std::size_t idx);
	SceneElement& get_prev_sibling();
	SceneElement& get_prev_sibling_bounded();
	SceneElement& get_next_sibling();
	SceneElement& get_next_sibling_bounded();
	SceneElement& get_parent();

	ScreenTexture& get_screen_texture(std::string name);
	ScreenTexture& get_screen_texture(std::size_t idx);
	ScreenText& get_screen_text(std::string name);
	ScreenText& get_screen_text(std::size_t idx);
	WorldTexture& get_world_texture(std::string name);
	WorldTexture& get_world_texture(std::size_t idx);
	WorldText& get_world_text(std::string name);
	WorldText& get_world_text(std::size_t idx);

	SceneElement& set_orientation(int orientation);
	int get_orientation() const;

	SceneElement& set_pos(glm::vec3 pos);
	SceneElement& set_pos(glm::vec3 pos, int frames);
	SceneElement& add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;

	SceneElement& set_rot(glm::vec3 rot);
	SceneElement& set_rot(glm::vec3 rot, int frames);
	SceneElement& add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	SceneElement& set_scale(glm::vec3 scale);
	SceneElement& set_scale(glm::vec3 scale, int frames);
	SceneElement& add_scale(glm::vec3 scale);
	glm::vec3 get_scale() const;

	SceneElement& show();
	SceneElement& hide();

	SceneElement& push_int_var(std::string name, int val);
	SceneElement& push_float_var(std::string name, float val);
	SceneElement& push_bool_var(std::string name, bool val);
	SceneElement& push_string_var(std::string name, std::string val);
	SceneElement& push_ptr_var(std::string name, void* val);
	int& int_var(std::string name);
	float& float_var(std::string name);
	bool& bool_var(std::string name);
	std::string& string_var(std::string name);
	void* ptr_var(std::string name);

	SceneElement& add_event(std::string event_name, std::function<void(SceneElement*)> func);
	SceneElement& execute_event(std::string event_name);

	SoundPlayer sound_player;

	void render();
private:
	std::string name;
	TextureAnchor anchor;
	SceneElement* parent;

	std::vector<SceneElement> children;
	std::map<std::string, std::size_t> child_map;
	
	std::vector<ScreenTexture> screen_textures;
	std::map<std::string, std::size_t> screen_texture_map;

	std::vector<ScreenText> screen_texts;
	std::map<std::string, std::size_t> screen_text_map;

	std::vector<WorldTexture> world_textures;
	std::map<std::string, std::size_t> world_texture_map;

	std::vector<WorldText> world_texts;
	std::map<std::string, std::size_t> world_text_map;

	std::vector<std::size_t> render_indices;

	std::map<std::string, std::function<void(SceneElement* elem)>> event_functions;

	std::map<std::string, int> int_vars;
	std::map<std::string, float> float_vars;
	std::map<std::string, bool> bool_vars;
	std::map<std::string, std::string> string_vars;
	std::map<std::string, void*> ptr_vars;

	int screen_orientation;
	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<glm::vec3> scale;
	bool visible;
};