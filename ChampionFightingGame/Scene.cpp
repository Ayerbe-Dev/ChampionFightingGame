#include "Scene.h"
#include "ObjectManager.h"
#include "FontManager.h"
#include "GameManager.h"

Scene::Scene() {
	active_element = &root;
	flags = 0;


}

Scene::~Scene() {

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
	GameManager::get_instance()->render_game_states();
}

void Scene::process_pre_event() {}
void Scene::process_main() {}
void Scene::render_main() {}

SceneElement& Scene::get_active_element() {
	return *active_element;
}

void Scene::set_active_element(SceneElement* element) {
	this->active_element = element;
}

void Scene::update_scene(unsigned int scene, unsigned int flags) {
	GameManager::get_instance()->update_state(scene, flags);
}

void Scene::execute_event(std::string event_name) {
	active_element->execute_event(event_name);
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