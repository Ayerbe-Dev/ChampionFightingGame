#include "SceneElement.h"
#include "GameManager.h"
#include "WindowConstants.h"
#include "utils.h"

SceneElement::SceneElement() {
	this->name = "root";
	this->parent = nullptr;
	this->screen_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0f);
	this->rot = glm::vec3(0.0f);
	this->scale = glm::vec3(1.0f);
	this->visible = true;
	this->render_event = false;
}

SceneElement::SceneElement(std::vector<std::pair<std::string, std::any>> elements) : SceneElement() {
	//LAZY TEST: Adding these lines noticeably decreases load times for a menu structure, but the
	//pause menu still loads far too slow
	children.reserve(elements.size());
	screen_textures.reserve(elements.size());
	screen_texts.reserve(elements.size());
	world_textures.reserve(elements.size());
	world_texts.reserve(elements.size());
	render_indices.reserve(elements.size()); //This one is actually just correct tho
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements[i].second.type() == typeid(SceneElement)) {
			child_map[elements[i].first] = children.size();
			children.push_back(std::any_cast<SceneElement>(elements[i].second));
			children.back().name = elements[i].first;
			children.back().parent = this;
			render_indices.push_back(ELEM_TYPE_ELEM);
			continue;
		}
		if (elements[i].second.type() == typeid(ScreenTexture)) {
			screen_texture_map[elements[i].first] = screen_textures.size();
			screen_textures.push_back(std::any_cast<ScreenTexture>(elements[i].second));
			screen_textures.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_SCRN_TEX);
			continue;
		}
		if (elements[i].second.type() == typeid(ScreenText)) {
			screen_text_map[elements[i].first] = screen_texts.size();
			screen_texts.push_back(std::any_cast<ScreenText>(elements[i].second));
			screen_texts.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_SCRN_TXT);
			continue;
		}
		if (elements[i].second.type() == typeid(WorldTexture)) {
			world_texture_map[elements[i].first] = world_textures.size();
			world_textures.push_back(std::any_cast<WorldTexture>(elements[i].second));
			world_textures.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_WRLD_TEX);
			continue;
		}
		if (elements[i].second.type() == typeid(WorldText)) {
			world_text_map[elements[i].first] = world_texts.size();
			world_texts.push_back(std::any_cast<WorldText>(elements[i].second));
			world_texts.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_WRLD_TXT);
			continue;
		}
		if (elements[i].second.type() == typeid(SceneElementLoop)) {
			SceneElementLoop loop = std::any_cast<SceneElementLoop>(elements[i].second);
			for (int i = 0; i < loop.n; i++) {
				loop.f(this, i);
			}
			continue;
		}
		std::cout << "Child " << elements[i].first << " of element " << name << " has invalid type " << elements[i].second.type().name() << "\n";
	}
}

SceneElement::SceneElement(SceneElement& other) {
	this->name = other.name;
	this->anchor = other.anchor;
	this->parent = other.parent;
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;
	this->children.reserve(other.children.size());
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
		this->children[i].parent = this;
	}
	this->child_map = other.child_map;
	this->screen_textures.reserve(other.screen_textures.size());
	for (size_t i = 0; i < other.screen_textures.size(); i++) {
		this->screen_textures.push_back(other.screen_textures[i]);
		this->screen_textures[i].set_anchor(&this->anchor);
	}
	this->screen_texture_map = other.screen_texture_map;
	this->screen_texts.reserve(other.screen_texts.size());
	for (size_t i = 0; i < other.screen_texts.size(); i++) {
		this->screen_texts.push_back(other.screen_texts[i]);
		this->screen_texts[i].set_anchor(&this->anchor);
	}
	this->screen_text_map = other.screen_text_map;
	this->world_textures.reserve(other.world_textures.size());
	for (size_t i = 0; i < other.world_textures.size(); i++) {
		this->world_textures.push_back(other.world_textures[i]);
		this->world_textures[i].set_anchor(&this->anchor);
	}
	this->world_texture_map = other.world_texture_map;
	this->world_texts.reserve(other.world_texts.size());
	for (size_t i = 0; i < other.world_texts.size(); i++) {
		this->world_texts.push_back(other.world_texts[i]);
		this->world_texts[i].set_anchor(&this->anchor);
	}
	this->world_text_map = other.world_text_map;
	this->render_indices.reserve(other.render_indices.size());
	for (size_t i = 0; i < other.render_indices.size(); i++) {
		this->render_indices.push_back(other.render_indices[i]);
	}
	this->event_functions = other.event_functions;
	this->screen_orientation = other.screen_orientation;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->visible = other.visible;
	this->render_event = other.render_event;
	this->sound_player = other.sound_player;
}

SceneElement::SceneElement(const SceneElement& other) {
	this->name = other.name;
	this->anchor = other.anchor;
	this->parent = other.parent;
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;
	this->children.reserve(other.children.size());
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
		this->children[i].parent = this;
	}
	this->child_map = other.child_map;
	this->screen_textures.reserve(other.screen_textures.size());
	for (size_t i = 0; i < other.screen_textures.size(); i++) {
		this->screen_textures.push_back(other.screen_textures[i]);
		this->screen_textures[i].set_anchor(&this->anchor);
	}
	this->screen_texture_map = other.screen_texture_map;
	this->screen_texts.reserve(other.screen_texts.size());
	for (size_t i = 0; i < other.screen_texts.size(); i++) {
		this->screen_texts.push_back(other.screen_texts[i]);
		this->screen_texts[i].set_anchor(&this->anchor);
	}
	this->screen_text_map = other.screen_text_map;
	this->world_textures.reserve(other.world_textures.size());
	for (size_t i = 0; i < other.world_textures.size(); i++) {
		this->world_textures.push_back(other.world_textures[i]);
		this->world_textures[i].set_anchor(&this->anchor);
	}
	this->world_texture_map = other.world_texture_map;
	this->world_texts.reserve(other.world_texts.size());
	for (size_t i = 0; i < other.world_texts.size(); i++) {
		this->world_texts.push_back(other.world_texts[i]);
		this->world_texts[i].set_anchor(&this->anchor);
	}
	this->world_text_map = other.world_text_map;
	this->render_indices.reserve(other.render_indices.size());
	for (size_t i = 0; i < other.render_indices.size(); i++) {
		this->render_indices.push_back(other.render_indices[i]);
	}
	this->event_functions = other.event_functions;
	this->screen_orientation = other.screen_orientation;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->visible = other.visible;
	this->render_event = other.render_event;
	this->sound_player = other.sound_player;
}

SceneElement::SceneElement(SceneElement&& other) noexcept {
	this->name = other.name;
	this->anchor = other.anchor;
	this->parent = other.parent;
	this->int_vars = other.int_vars;
	this->float_vars = other.float_vars;
	this->bool_vars = other.bool_vars;
	this->string_vars = other.string_vars;
	this->ptr_vars = other.ptr_vars;
	this->children.reserve(other.children.size());
	for (size_t i = 0; i < other.children.size(); i++) {
		this->children.push_back(other.children[i]);
		this->children[i].parent = this;
	}
	this->child_map = other.child_map;
	this->screen_textures.reserve(other.screen_textures.size());
	for (size_t i = 0; i < other.screen_textures.size(); i++) {
		this->screen_textures.push_back(other.screen_textures[i]);
		this->screen_textures[i].set_anchor(&this->anchor);
	}
	this->screen_texture_map = other.screen_texture_map;
	this->screen_texts.reserve(other.screen_texts.size());
	for (size_t i = 0; i < other.screen_texts.size(); i++) {
		this->screen_texts.push_back(other.screen_texts[i]);
		this->screen_texts[i].set_anchor(&this->anchor);
	}
	this->screen_text_map = other.screen_text_map;
	this->world_textures.reserve(other.world_textures.size());
	for (size_t i = 0; i < other.world_textures.size(); i++) {
		this->world_textures.push_back(other.world_textures[i]);
		this->world_textures[i].set_anchor(&this->anchor);
	}
	this->world_texture_map = other.world_texture_map;
	this->world_texts.reserve(other.world_texts.size());
	for (size_t i = 0; i < other.world_texts.size(); i++) {
		this->world_texts.push_back(other.world_texts[i]);
		this->world_texts[i].set_anchor(&this->anchor);
	}
	this->world_text_map = other.world_text_map;
	this->render_indices.reserve(other.render_indices.size());
	for (size_t i = 0; i < other.render_indices.size(); i++) {
		this->render_indices.push_back(other.render_indices[i]);
	}
	this->event_functions = other.event_functions;
	this->screen_orientation = other.screen_orientation;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->visible = other.visible;
	this->render_event = other.render_event;
	this->sound_player = other.sound_player;
}

SceneElement& SceneElement::operator=(SceneElement& other) {
	if (this != &other) {
		this->name = other.name;
		this->anchor = other.anchor;
		this->parent = other.parent;
		this->int_vars = other.int_vars;
		this->float_vars = other.float_vars;
		this->bool_vars = other.bool_vars;
		this->string_vars = other.string_vars;
		this->ptr_vars = other.ptr_vars;
		this->children.reserve(other.children.size());
		for (size_t i = 0; i < other.children.size(); i++) {
			this->children.push_back(other.children[i]);
			this->children[i].parent = this;
		}
		this->child_map = other.child_map;
		this->screen_textures.reserve(other.screen_textures.size());
		for (size_t i = 0; i < other.screen_textures.size(); i++) {
			this->screen_textures.push_back(other.screen_textures[i]);
			this->screen_textures[i].set_anchor(&this->anchor);
		}
		this->screen_texture_map = other.screen_texture_map;
		this->screen_texts.reserve(other.screen_texts.size());
		for (size_t i = 0; i < other.screen_texts.size(); i++) {
			this->screen_texts.push_back(other.screen_texts[i]);
			this->screen_texts[i].set_anchor(&this->anchor);
		}
		this->screen_text_map = other.screen_text_map;
		this->world_textures.reserve(other.world_textures.size());
		for (size_t i = 0; i < other.world_textures.size(); i++) {
			this->world_textures.push_back(other.world_textures[i]);
			this->world_textures[i].set_anchor(&this->anchor);
		}
		this->world_texture_map = other.world_texture_map;
		this->world_texts.reserve(other.world_texts.size());
		for (size_t i = 0; i < other.world_texts.size(); i++) {
			this->world_texts.push_back(other.world_texts[i]);
			this->world_texts[i].set_anchor(&this->anchor);
		}
		this->world_text_map = other.world_text_map;
		this->render_indices.reserve(other.render_indices.size());
		for (size_t i = 0; i < other.render_indices.size(); i++) {
			this->render_indices.push_back(other.render_indices[i]);
		}
		this->event_functions = other.event_functions;
		this->screen_orientation = other.screen_orientation;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->visible = other.visible;
		this->render_event = other.render_event;
		this->sound_player = other.sound_player;
	}
	return *this;
}

SceneElement& SceneElement::operator=(const SceneElement& other) {
	if (this != &other) {
		this->name = other.name;
		this->anchor = other.anchor;
		this->parent = other.parent;
		this->int_vars = other.int_vars;
		this->float_vars = other.float_vars;
		this->bool_vars = other.bool_vars;
		this->string_vars = other.string_vars;
		this->ptr_vars = other.ptr_vars;
		this->children.reserve(other.children.size());
		for (size_t i = 0; i < other.children.size(); i++) {
			this->children.push_back(other.children[i]);
			this->children[i].parent = this;
		}
		this->child_map = other.child_map;
		this->screen_textures.reserve(other.screen_textures.size());
		for (size_t i = 0; i < other.screen_textures.size(); i++) {
			this->screen_textures.push_back(other.screen_textures[i]);
			this->screen_textures[i].set_anchor(&this->anchor);
		}
		this->screen_texture_map = other.screen_texture_map;
		this->screen_texts.reserve(other.screen_texts.size());
		for (size_t i = 0; i < other.screen_texts.size(); i++) {
			this->screen_texts.push_back(other.screen_texts[i]);
			this->screen_texts[i].set_anchor(&this->anchor);
		}
		this->screen_text_map = other.screen_text_map;
		this->world_textures.reserve(other.world_textures.size());
		for (size_t i = 0; i < other.world_textures.size(); i++) {
			this->world_textures.push_back(other.world_textures[i]);
			this->world_textures[i].set_anchor(&this->anchor);
		}
		this->world_texture_map = other.world_texture_map;
		this->world_texts.reserve(other.world_texts.size());
		for (size_t i = 0; i < other.world_texts.size(); i++) {
			this->world_texts.push_back(other.world_texts[i]);
			this->world_texts[i].set_anchor(&this->anchor);
		}
		this->world_text_map = other.world_text_map;
		this->render_indices.reserve(other.render_indices.size());
		for (size_t i = 0; i < other.render_indices.size(); i++) {
			this->render_indices.push_back(other.render_indices[i]);
		}
		this->event_functions = other.event_functions;
		this->screen_orientation = other.screen_orientation;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->visible = other.visible;
		this->render_event = other.render_event;
		this->sound_player = other.sound_player;
	}
	return *this;
}

SceneElement& SceneElement::operator=(SceneElement&& other) noexcept {
	if (this != &other) {
		this->name = other.name;
		this->anchor = other.anchor;
		this->parent = other.parent;
		this->int_vars = other.int_vars;
		this->float_vars = other.float_vars;
		this->bool_vars = other.bool_vars;
		this->string_vars = other.string_vars;
		this->ptr_vars = other.ptr_vars;
		this->children.reserve(other.children.size());
		for (size_t i = 0; i < other.children.size(); i++) {
			this->children.push_back(other.children[i]);
			this->children[i].parent = this;
		}
		this->child_map = other.child_map;
		this->screen_textures.reserve(other.screen_textures.size());
		for (size_t i = 0; i < other.screen_textures.size(); i++) {
			this->screen_textures.push_back(other.screen_textures[i]);
			this->screen_textures[i].set_anchor(&this->anchor);
		}
		this->screen_texture_map = other.screen_texture_map;
		this->screen_texts.reserve(other.screen_texts.size());
		for (size_t i = 0; i < other.screen_texts.size(); i++) {
			this->screen_texts.push_back(other.screen_texts[i]);
			this->screen_texts[i].set_anchor(&this->anchor);
		}
		this->screen_text_map = other.screen_text_map;
		this->world_textures.reserve(other.world_textures.size());
		for (size_t i = 0; i < other.world_textures.size(); i++) {
			this->world_textures.push_back(other.world_textures[i]);
			this->world_textures[i].set_anchor(&this->anchor);
		}
		this->world_texture_map = other.world_texture_map;
		this->world_texts.reserve(other.world_texts.size());
		for (size_t i = 0; i < other.world_texts.size(); i++) {
			this->world_texts.push_back(other.world_texts[i]);
			this->world_texts[i].set_anchor(&this->anchor);
		}
		this->world_text_map = other.world_text_map;
		this->render_indices.reserve(other.render_indices.size());
		for (size_t i = 0; i < other.render_indices.size(); i++) {
			this->render_indices.push_back(other.render_indices[i]);
		}
		this->event_functions = other.event_functions;
		this->screen_orientation = other.screen_orientation;
		this->pos = other.pos;
		this->rot = other.rot;
		this->scale = other.scale;
		this->visible = other.visible;
		this->render_event = other.render_event;
		this->sound_player = other.sound_player;
	}
	return *this;
}

SceneElement::~SceneElement() {

}

int SceneElement::get_id() const {
	if (parent) {
		return parent->child_map[name];
	}
	return -1;
}

std::string SceneElement::get_name() const {
	return this->name;
}

std::string SceneElement::get_full_name() const {
	std::string addr = "";
	if (parent) {
		addr = parent->get_full_name() + "/";
	}
	return addr + this->name;
}


SceneElement& SceneElement::add_element(std::string name, std::any element) {
	if (element.type() == typeid(SceneElement)) {
		child_map[name] = children.size();
		children.push_back(std::any_cast<SceneElement>(element));
		children.back().name = name;
		children.back().parent = this;
		render_indices.push_back(ELEM_TYPE_ELEM);
		return *this;
	}
	if (element.type() == typeid(ScreenTexture)) {
		screen_texture_map[name] = screen_textures.size();
		screen_textures.push_back(std::any_cast<ScreenTexture>(element));
		screen_textures.back().set_anchor(&this->anchor);
		render_indices.push_back(ELEM_TYPE_SCRN_TEX);
		return *this;
	}
	if (element.type() == typeid(ScreenText)) {
		screen_text_map[name] = screen_texts.size();
		screen_texts.push_back(std::any_cast<ScreenText>(element));
		screen_texts.back().set_anchor(&this->anchor);
		render_indices.push_back(ELEM_TYPE_SCRN_TXT);
		return *this;
	}
	if (element.type() == typeid(WorldTexture)) {
		world_texture_map[name] = world_textures.size();
		world_textures.push_back(std::any_cast<WorldTexture>(element));
		world_textures.back().set_anchor(&this->anchor);
		render_indices.push_back(ELEM_TYPE_WRLD_TEX);
		return *this;
	}
	if (element.type() == typeid(WorldText)) {
		world_text_map[name] = world_texts.size();
		world_texts.push_back(std::any_cast<WorldText>(element));
		world_texts.back().set_anchor(&this->anchor);
		render_indices.push_back(ELEM_TYPE_WRLD_TXT);
		return *this;
	}
	if (element.type() == typeid(SceneElementLoop)) {
		SceneElementLoop loop = std::any_cast<SceneElementLoop>(element);
		for (int i = 0; i < loop.n; i++) {
			loop.f(this, i);
		}
		return *this;
	}
	std::cout << "Child " << name << " of element " << this->name << " has invalid type << " << element.type().name() << "\n";
	return *this;
}

SceneElement& SceneElement::add_elements(std::vector<std::pair<std::string, std::any>> elements) {
	children.reserve(children.size() + elements.size());
	screen_textures.reserve(screen_textures.size() + elements.size());
	screen_texts.reserve(screen_texts.size() + elements.size());
	world_textures.reserve(world_textures.size() + elements.size());
	world_texts.reserve(world_texts.size() + elements.size());
	render_indices.reserve(render_indices.size() + elements.size());
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements[i].second.type() == typeid(SceneElement)) {
			child_map[elements[i].first] = children.size();
			children.push_back(std::any_cast<SceneElement>(elements[i].second));
			children.back().name = elements[i].first;
			children.back().parent = this;
			render_indices.push_back(ELEM_TYPE_ELEM);
			continue;
		}
		if (elements[i].second.type() == typeid(ScreenTexture)) {
			screen_texture_map[elements[i].first] = screen_textures.size();
			screen_textures.push_back(std::any_cast<ScreenTexture>(elements[i].second));
			screen_textures.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_SCRN_TEX);
			continue;
		}
		if (elements[i].second.type() == typeid(ScreenText)) {
			screen_text_map[elements[i].first] = screen_texts.size();
			screen_texts.push_back(std::any_cast<ScreenText>(elements[i].second));
			screen_texts.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_SCRN_TXT);
			continue;
		}
		if (elements[i].second.type() == typeid(WorldTexture)) {
			world_texture_map[elements[i].first] = world_textures.size();
			world_textures.push_back(std::any_cast<WorldTexture>(elements[i].second));
			world_textures.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_WRLD_TEX);
			continue;
		}
		if (elements[i].second.type() == typeid(WorldText)) {
			world_text_map[elements[i].first] = world_texts.size();
			world_texts.push_back(std::any_cast<WorldText>(elements[i].second));
			world_texts.back().set_anchor(&this->anchor);
			render_indices.push_back(ELEM_TYPE_WRLD_TXT);
			continue;
		}
		if (elements[i].second.type() == typeid(SceneElementLoop)) {
			SceneElementLoop loop = std::any_cast<SceneElementLoop>(elements[i].second);
			for (int i = 0; i < loop.n; i++) {
				loop.f(this, i);
			}
			continue;
		}
		std::cout << "Child " << elements[i].first << " of element " << name << " has invalid type << " << elements[i].second.type().name() << "\n";
	}
	return *this;
}

SceneElement& SceneElement::get_child(std::string name) {
	std::size_t rec = name.find("/");
	if (rec != name.npos) {
		return get_child(name.substr(0, rec)).get_child(name.substr(rec + 1, name.npos));
	}
	return children[child_map[name]];
}

SceneElement& SceneElement::get_child(std::size_t idx) {
	return children[idx];
}

SceneElement& SceneElement::get_sibling(std::string name) {
	if (parent) {
		return parent->get_child(name);
	}
	GameManager::get_instance()->add_crash_log("Called get_sibling() as root element");
	return *this;
}

SceneElement& SceneElement::get_sibling(std::size_t idx) {
	if (parent) {
		return parent->children[(parent->child_map[name] - 1) % parent->children.size()];
	}
	GameManager::get_instance()->add_crash_log("Called get_sibling() as root element");
	return *this;
}

SceneElement& SceneElement::get_sibling_bounded(std::size_t idx) {
	if (parent) {
		return parent->children[clamp(0, (parent->child_map[name] - 1), parent->children.size() - 1)];
	}
	GameManager::get_instance()->add_crash_log("Called get_sibling() as root element");
	return *this;
}

SceneElement& SceneElement::get_prev_sibling() {
	if (parent) {
		if (!parent->child_map[name]) return parent->children[parent->children.size() - 1];
		return parent->children[parent->child_map[name] - 1];
	}
	GameManager::get_instance()->add_crash_log("Called get_prev_sibling() as root element");
	return *this;
}

SceneElement& SceneElement::get_prev_sibling_bounded() {
	if (parent) {
		return parent->children[std::max((int)parent->child_map[name] - 1, 0)];
	}
	GameManager::get_instance()->add_crash_log("Called get_prev_sibling_bounded() as root element");
	return *this;
}

SceneElement& SceneElement::get_next_sibling() {
	if (parent) {
		return parent->children[(parent->child_map[name] + 1) % parent->children.size()];
	}
	GameManager::get_instance()->add_crash_log("Called get_next_sibling() as root element");
	return *this;
}

SceneElement& SceneElement::get_next_sibling_bounded() {
	if (parent) {
		return parent->children[std::min(parent->child_map[name] + 1, parent->children.size() - 1)];
	}
	GameManager::get_instance()->add_crash_log("Called get_next_sibling_bounded() as root element");
	return *this;
}

SceneElement& SceneElement::get_parent() {
	if (parent) {
		return *parent;
	}
	GameManager::get_instance()->add_crash_log("Called get_parent() as root element");
	return *this;
}

std::size_t SceneElement::get_num_children() const {
	return children.size();
}

ScreenTexture& SceneElement::get_screen_texture(std::string name) {
	std::size_t rec = name.find("/");
	if (rec != name.npos) {
		return get_child(name.substr(0, rec)).get_screen_texture(name.substr(rec + 1, name.npos));
	}
	return screen_textures[screen_texture_map[name]];
}

ScreenTexture& SceneElement::get_screen_texture(std::size_t idx) {
	return screen_textures[idx];
}

std::size_t SceneElement::get_num_screen_textures() const {
	return screen_textures.size();
}

ScreenText& SceneElement::get_screen_text(std::string name) {
	std::size_t rec = name.find("/");
	if (rec != name.npos) {
		return get_child(name.substr(0, rec)).get_screen_text(name.substr(rec + 1, name.npos));
	}
	if (!screen_text_map.contains(name)) {
		std::cout << "Couldn't find " << name << "\n";
	}
	return screen_texts[screen_text_map[name]];
}

ScreenText& SceneElement::get_screen_text(std::size_t idx) {
	return screen_texts[idx];
}

std::size_t SceneElement::get_num_screen_texts() const {
	return screen_texts.size();
}

WorldTexture& SceneElement::get_world_texture(std::string name) {
	std::size_t rec = name.find("/");
	if (rec != name.npos) {
		return get_child(name.substr(0, rec)).get_world_texture(name.substr(rec + 1, name.npos));
	}
	return world_textures[world_texture_map[name]];
}

WorldTexture& SceneElement::get_world_texture(std::size_t idx) {
	return world_textures[idx];
}

std::size_t SceneElement::get_num_world_textures() const {
	return world_textures.size();
}

WorldText& SceneElement::get_world_text(std::string name) {
	std::size_t rec = name.find("/");
	if (rec != name.npos) {
		return get_child(name.substr(0, rec)).get_world_text(name.substr(rec + 1, name.npos));
	}
	return world_texts[world_text_map[name]];
}

WorldText& SceneElement::get_world_text(std::size_t idx) {
	return world_texts[idx];
}

std::size_t SceneElement::get_num_world_texts() const {
	return world_texts.size();
}

SceneElement& SceneElement::set_orientation(int orientation) {
	this->screen_orientation = orientation;
	return *this;
}

int SceneElement::get_orientation() const {
	return screen_orientation;
}

SceneElement& SceneElement::set_pos(glm::vec3 pos) {
	this->pos = pos;
	return *this;
}

SceneElement& SceneElement::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
	return *this;
}

SceneElement& SceneElement::add_pos(glm::vec3 pos) {
	this->pos += pos;
	return *this;
}

glm::vec3 SceneElement::get_pos() const {
	return pos;
}

SceneElement& SceneElement::set_rot(glm::vec3 rot) {
	this->rot = rot;
	return *this;
}

SceneElement& SceneElement::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
	return *this;
}

SceneElement& SceneElement::add_rot(glm::vec3 rot) {
	this->rot += rot;
	return *this;
}

glm::vec3 SceneElement::get_rot() const {
	return rot;
}

SceneElement& SceneElement::set_scale(glm::vec3 scale) {
	this->scale = scale;
	return *this;
}

SceneElement& SceneElement::set_scale(glm::vec3 scale, int frames) {
	this->scale.set_target_val(scale, frames);
	return *this;
}

SceneElement& SceneElement::add_scale(glm::vec3 scale) {
	this->scale += scale;
	return *this;
}

glm::vec3 SceneElement::get_scale() const {
	return scale;
}

SceneElement& SceneElement::set_anchor_dimensions(float w, float h) {
	this->anchor.local_w = w / WINDOW_WIDTH;
	this->anchor.local_h = h / WINDOW_HEIGHT;
	return *this;
}

SceneElement& SceneElement::show() {
	this->visible = true;
	return *this;
}

SceneElement& SceneElement::hide() {
	this->visible = false;
	return *this;
}

bool SceneElement::is_visible() const {
	return this->visible;
}

SceneElement& SceneElement::int_var(std::string name, int val) {
	int_vars[name] = val;
	return *this;
}

SceneElement& SceneElement::float_var(std::string name, float val) {
	float_vars[name] = val;
	return *this;
}

SceneElement& SceneElement::bool_var(std::string name, bool val) {
	bool_vars[name] = val;
	return *this;
}

SceneElement& SceneElement::string_var(std::string name, std::string val) {
	string_vars[name] = val;
	return *this;
}

SceneElement& SceneElement::ptr_var(std::string name, void* val) {
	ptr_vars[name] = val;
	return *this;
}

int& SceneElement::int_var(std::string name) {
	return int_vars[name];
}

float& SceneElement::float_var(std::string name) {
	return float_vars[name];
}

bool& SceneElement::bool_var(std::string name) {
	return bool_vars[name];
}

std::string& SceneElement::string_var(std::string name) {
	return string_vars[name];
}

void* SceneElement::ptr_var(std::string name) {
	return ptr_vars[name];
}

SceneElement& SceneElement::add_event(std::string event_name, std::function<void(SceneElement*)> func) {
	event_functions[event_name] = func;
	return *this;
}

SceneElement& SceneElement::execute_event(std::string event_name) {
	if (event_functions.contains(event_name)) {
		event_functions[event_name](this);
	}
	return *this;
}

SceneElement& SceneElement::load_sound_list(std::string list, std::string dir) {
	sound_player.load_sound_list(list, dir);
	return *this;
}

SceneElement& SceneElement::load_sound(std::string name, std::string dir) {
	sound_player.load_sound(name, dir);
	return *this;
}

void SceneElement::render() {
	if (!visible) return;
	if (!render_event) {
		render_event = true;
		execute_event("on_render");
		render_event = false;
	}
	glm::vec3 screen_pos = pos.get_val();
	anchor.w = anchor.local_w;
	anchor.h = anchor.local_h;
	anchor.screen_mat = glm::mat4(1.0f);
	anchor.world_mat = glm::mat4(1.0f);
	if (parent) {
		anchor.w *= parent->anchor.w;
		anchor.h *= parent->anchor.h;
		anchor.screen_mat = parent->anchor.screen_mat;
		anchor.world_mat = parent->anchor.world_mat;
	}
	float window_width = WINDOW_WIDTH * anchor.w;
	float window_height = WINDOW_HEIGHT * anchor.h;
	if (screen_orientation & TEXTURE_LEFT) {
		screen_pos.x -= window_width;
	}
	else if (screen_orientation & TEXTURE_RIGHT) {
		screen_pos.x *= -1.0;
		screen_pos.x += window_width;
	}
	if (screen_orientation & TEXTURE_TOP) {
		screen_pos.y *= -1.0;
		screen_pos.y += window_height;
	}
	else if (screen_orientation & TEXTURE_BOTTOM) {
		screen_pos.y -= window_height;
	}
	screen_pos /= glm::vec3(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_DEPTH
	);
	anchor.screen_mat = glm::translate(anchor.screen_mat, screen_pos);
	anchor.screen_mat = glm::rotate(anchor.screen_mat, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	anchor.screen_mat = glm::rotate(anchor.screen_mat, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	anchor.screen_mat = glm::rotate(anchor.screen_mat, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));
	anchor.screen_mat = glm::scale(anchor.screen_mat, scale.get_val());

	glm::vec3 world_pos = pos.get_val() / glm::vec3(
		WINDOW_WIDTH / 100.0f,
		WINDOW_HEIGHT / 100.0f,
		WINDOW_DEPTH / 100.0f
	);
	anchor.world_mat = glm::scale(anchor.world_mat, scale.get_val());
	anchor.world_mat = glm::translate(anchor.world_mat, world_pos);
	anchor.world_mat = glm::rotate(anchor.world_mat, glm::radians(rot.get_val().x), glm::vec3(1.0, 0.0, 0.0));
	anchor.world_mat = glm::rotate(anchor.world_mat, glm::radians(rot.get_val().y), glm::vec3(0.0, 1.0, 0.0));
	anchor.world_mat = glm::rotate(anchor.world_mat, glm::radians(rot.get_val().z), glm::vec3(0.0, 0.0, 1.0));

	std::size_t idx[5] = {0, 0, 0, 0, 0};
	for (size_t i = 0; i < render_indices.size(); i++) {
		switch (render_indices[i]) {
			case ELEM_TYPE_ELEM: {
				children[idx[render_indices[i]]].render();
			} break;
			case ELEM_TYPE_SCRN_TEX: {
				screen_textures[idx[render_indices[i]]].render();
			} break;
			case ELEM_TYPE_SCRN_TXT: {
				screen_texts[idx[render_indices[i]]].render();
			} break;
			case ELEM_TYPE_WRLD_TEX: {
				world_textures[idx[render_indices[i]]].render();
			} break;
			case ELEM_TYPE_WRLD_TXT: {
				world_texts[idx[render_indices[i]]].render();
			} break;
		}
		idx[render_indices[i]]++;
	}
}

void SceneElement::update_anchor_dimensions() {
	this->anchor.w = this->anchor.local_w * parent->anchor.w;
	this->anchor.h = this->anchor.local_h * parent->anchor.h;
}

SceneElementLoop::SceneElementLoop() {
	this->n = 0;
}

SceneElementLoop::SceneElementLoop(int n, std::function<void(SceneElement*, int)> f) {
	this->n = n;
	this->f = f;
}

SceneElementLoop::SceneElementLoop(const SceneElementLoop& other) {
	this->n = other.n;
	this->f = other.f;
}

SceneElementLoop& SceneElementLoop::operator=(const SceneElementLoop& other) {
	if (this != &other) {
		this->n = other.n;
		this->f = other.f;
	}
	return *this;
}

SceneElementLoop::~SceneElementLoop() {

}