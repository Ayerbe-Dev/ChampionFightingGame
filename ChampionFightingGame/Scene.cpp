#include "Scene.h"
#include "ObjectManager.h"
#include "FontManager.h"
#include "GameManager.h"

Scene::Scene() {
	active_element[0] = &root;
	active_element[1] = &root;
	context = 0;
	player_id = 0;
	sub_scene = SCENE_NONE;
	GameManager* game_manager = GameManager::get_instance();
	for (int i = 0; i < 2; i++) {
		player[i] = game_manager->player[i];
	}
	game_manager->set_scene(this);
	looping = true;

}

Scene::~Scene() {
	for (auto& f : font_map) {
		f.second.unload_font();
	}
	GameManager* game_manager = GameManager::get_instance();
	game_manager->delete_scene();
}

void Scene::process() {
	mouse.poll_buttons();
	for (int i = 0; i < 2; i++) {
		player[i]->poll_controller_menu();
	}
	process_pre_event();
	GameManager::get_instance()->process_events();
	process_main();
	ObjectManager::get_instance()->process_world_frame();
	SoundManager::get_instance()->process_sounds();
}

void Scene::render() {
	GameManager::get_instance()->render();
}

void Scene::process_pre_event() {}
void Scene::process_main() {}
void Scene::render_main() {}

SceneElement& Scene::get_active_element() {
	return *active_element[player_id];
}

void Scene::set_active_element(SceneElement* element) {
	active_element[0]->execute_event("deactivate");
	active_element[0] = element;
	active_element[1] = element;
	active_element[0]->execute_event("activate");
}

void Scene::set_player_active_element(SceneElement* element) {
	active_element[player_id]->execute_event("deactivate");
	active_element[player_id] = element;
	active_element[player_id]->execute_event("activate");
}

SceneElement& Scene::get_element(std::string name) {
	if (name.starts_with("root/")) {
		name = name.substr(5, name.npos);
	}
	return root.get_child(name);
}

ScreenTexture& Scene::get_screen_texture(std::string name) {
	if (name.starts_with("root/")) {
		name = name.substr(5, name.npos);
	}
	return root.get_screen_texture(name);
}

ScreenText& Scene::get_screen_text(std::string name) {
	if (name.starts_with("root/")) {
		name = name.substr(5, name.npos);
	}
	return root.get_screen_text(name);
}

WorldTexture& Scene::get_world_texture(std::string name) {
	if (name.starts_with("root/")) {
		name = name.substr(5, name.npos);
	}
	return root.get_world_texture(name);
}

WorldText& Scene::get_world_text(std::string name) {
	if (name.starts_with("root/")) {
		name = name.substr(5, name.npos);
	}
	return root.get_world_text(name);
}

void Scene::update_scene(unsigned int scene, unsigned int context) {
	GameManager::get_instance()->update_scene(scene, context);
}

void Scene::execute_event(std::string event_name) {
	active_element[player_id]->execute_event(event_name);
}

void Scene::load_font(std::string name, std::string font_name, int font_size) {
	if (font_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Font " + name + " already loaded");
		return;
	}
	font_map[name] = FontManager::get_instance()->load_font(font_name, font_size);
}

Font& Scene::get_font(std::string name) {
	if (!font_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Font " + name + " not loaded");
	}
	return font_map[name];
}

void Scene::load_event(std::string name, std::function<void(SceneElement*)> fn) {
	if (event_map.contains(name)) {
		GameManager::get_instance()->add_crash_log("Event " + name + " already loaded");
		return;
	}
	event_map[name] = fn;
}

std::function<void(SceneElement*)> Scene::get_event(std::string name, bool allow_unloaded) {
	if (!event_map.contains(name)) {
		if (!allow_unloaded) {
			GameManager::get_instance()->add_crash_log("Event " + name + " not loaded");
		}
		event_map[name] = [this](SceneElement* e) {};
	}
	return event_map[name];
}