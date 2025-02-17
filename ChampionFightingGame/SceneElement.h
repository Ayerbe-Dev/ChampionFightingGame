#pragma once
#include "ScreenTexture.h"
#include "ScreenText.h"
#include "WorldTexture.h"
#include "WorldText.h"
#include "SoundPlayer.h"
#include <any>
#include <vector>
#include <unordered_map>
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

	int get_id() const;
	std::string get_name() const;
	std::string get_full_name() const;

	SceneElement& add_element(std::string name, std::any element);
	SceneElement& add_elements(std::vector<std::pair<std::string, std::any>> elements);

	SceneElement& get_child(std::string name);
	SceneElement& get_child(std::size_t idx);
	SceneElement& get_sibling(std::string name);
	SceneElement& get_sibling(std::size_t idx);
	SceneElement& get_sibling_bounded(std::size_t idx);
	SceneElement& get_prev_sibling();
	SceneElement& get_prev_sibling_bounded();
	SceneElement& get_next_sibling();
	SceneElement& get_next_sibling_bounded();
	SceneElement& get_parent();

	std::size_t get_num_children() const;

	ScreenTexture& get_screen_texture(std::string name);
	ScreenTexture& get_screen_texture(std::size_t idx);
	std::size_t get_num_screen_textures() const;
	ScreenText& get_screen_text(std::string name);
	ScreenText& get_screen_text(std::size_t idx);
	std::size_t get_num_screen_texts() const;
	WorldTexture& get_world_texture(std::string name);
	WorldTexture& get_world_texture(std::size_t idx);
	std::size_t get_num_world_textures() const;
	WorldText& get_world_text(std::string name);
	WorldText& get_world_text(std::size_t idx);
	std::size_t get_num_world_texts() const;

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

	SceneElement& set_anchor_dimensions(float w, float h);

	SceneElement& show();
	SceneElement& hide();
	bool is_visible() const;

	SceneElement& int_var(std::string name, int val);
	SceneElement& float_var(std::string name, float val);
	SceneElement& bool_var(std::string name, bool val);
	SceneElement& string_var(std::string name, std::string val);
	SceneElement& ptr_var(std::string name, void* val);
	int& int_var(std::string name);
	float& float_var(std::string name);
	bool& bool_var(std::string name);
	std::string& string_var(std::string name);
	void* ptr_var(std::string name);

	SceneElement& add_event(std::string event_name, std::function<void(SceneElement*)> func);
	SceneElement& execute_event(std::string event_name);

	SceneElement& load_sound_list(std::string list, std::string dir);
	SceneElement& load_sound(std::string name, std::string dir);

	SoundPlayer sound_player;

	void render();
private:
	void update_anchor_dimensions();

	std::string name;
	TextureAnchor anchor;
	SceneElement* parent;

	std::vector<SceneElement> children;
	std::unordered_map<std::string, std::size_t> child_map;
	
	std::vector<ScreenTexture> screen_textures;
	std::unordered_map<std::string, std::size_t> screen_texture_map;

	std::vector<ScreenText> screen_texts;
	std::unordered_map<std::string, std::size_t> screen_text_map;

	std::vector<WorldTexture> world_textures;
	std::unordered_map<std::string, std::size_t> world_texture_map;

	std::vector<WorldText> world_texts;
	std::unordered_map<std::string, std::size_t> world_text_map;

	std::vector<std::size_t> render_indices;

	std::unordered_map<std::string, std::function<void(SceneElement* elem)>> event_functions;

	std::unordered_map<std::string, int> int_vars;
	std::unordered_map<std::string, float> float_vars;
	std::unordered_map<std::string, bool> bool_vars;
	std::unordered_map<std::string, std::string> string_vars;
	std::unordered_map<std::string, void*> ptr_vars;

	int screen_orientation;
	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<glm::vec3> scale;
	bool visible;
	bool render_event;
};

struct SceneElementLoop {
	SceneElementLoop();
	SceneElementLoop(int n, std::function<void(SceneElement*, int)> f);
	SceneElementLoop(const SceneElementLoop& other);
	SceneElementLoop& operator=(const SceneElementLoop& other);
	~SceneElementLoop();

	int n;
	std::function<void(SceneElement*, int)> f;
};