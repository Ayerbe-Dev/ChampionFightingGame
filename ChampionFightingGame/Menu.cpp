#pragma warning(disable : 4996)
#include "Menu.h"
#include <glew/glew.h>
#include "utils.h"
#include "Player.h"
#include <math.h>
#include "Debugger.h"
#include "DebugMenu.h"
#include "Options.h"
#include "Loader.h"
#include "RenderManager.h"
#include "FontManager.h"
#include "SoundManager.h"

void menu_main() {
	GameManager* game_manager = GameManager::get_instance();
	RenderManager* render_manager = RenderManager::get_instance();
	FontManager* font_manager = FontManager::get_instance();
	
	font_manager->load_face("Fiend-Oblique");

	Player *player[2];
	player[0] = game_manager->player[0];
	player[1] = game_manager->player[1];
	const Uint8* keyboard_state;

	MainMenu *main_menu = new MainMenu;
	main_menu->load_game_menu();

	while (*main_menu->looping) {
		wait_ms();
		for (int i = 0; i < 2; i++) {
			player[i]->controller.check_controllers();
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game_manager->handle_window_events();

		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player[i]->controller.poll_buttons(keyboard_state);
		}
		
		game_manager->handle_menus();
		main_menu->process_main();
		main_menu->render();

		SDL_GL_SwapWindow(render_manager->window);

		if (main_menu->sub_state != GAME_SUBSTATE_NONE) {
			if (game_manager->game_substate_main[main_menu->sub_state] != nullptr) {
				game_manager->game_substate_main[main_menu->sub_state]();
			}
			else {
				game_manager->add_crash_log("Error: Game Substate was " + std::to_string(main_menu->sub_state) + " (not GAME_SUBSTATE_NONE) but there was no associated function!");
				game_manager->update_state(GAME_STATE_DEBUG_MENU);
			}
			main_menu->sub_state = GAME_SUBSTATE_NONE;
		}
	}

	font_manager->unload_face("Fiend-Oblique");
	delete main_menu;
}

MainMenu::MainMenu() {
	theta = 0;
	offset = 3.14 / 11;
	magnitude = WINDOW_WIDTH / 1.125;
	turn_frames = 8;
	menu_frame = 0;
}

MainMenu::~MainMenu() {
	for (int i = 0; i < 5; i++) {

	}
	table.destroy();
	delete game_loader;
}

void MainMenu::load_game_menu() {
	GameManager* game_manager = GameManager::get_instance();
	game_manager->set_menu_info(this);

	RenderManager* render_manager = RenderManager::get_instance();

	game_loader = new GameLoader(1);
	std::thread loading_thread(&GameLoader::loading_screen, game_loader);
	loading_thread.detach();

	FontManager* font_manager = FontManager::get_instance();
	Font main_text_font = font_manager->load_font("Fiend-Oblique", 136);
	Font sub_text_font = font_manager->load_font("Fiend-Oblique", 100);
	glm::vec4 rgba = { 255.0, 127.0, 0.0, 255.0 };
	float x_coord = 12.0;
	float y_coord = -12.0;

	menu_objects.resize(2);

	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ALWAYS].push_back(MenuObject(this, nullptr, true)); 
		//This menu object group handles text and everything that generally always renders
		
		menu_objects[MENU_GROUP_RENDER_ACTIVE].push_back(MenuObject(this, nullptr, true)); 
		//This menu object group handles the bg image and sub menu contents, basically the stuff that
		//only renders based on what you selected
	}

	//Add the rotating text to the "render no matter what" group

	menu_objects[MENU_GROUP_RENDER_ALWAYS][SUB_MENU_ONLINE].add_texture(main_text_font, "Online", rgba, x_coord, y_coord);
	menu_objects[MENU_GROUP_RENDER_ALWAYS][SUB_MENU_SOLO].add_texture(main_text_font, "Solo", rgba, x_coord, y_coord);
	menu_objects[MENU_GROUP_RENDER_ALWAYS][SUB_MENU_VS].add_texture(main_text_font, "VS", rgba, x_coord, y_coord);
	menu_objects[MENU_GROUP_RENDER_ALWAYS][SUB_MENU_OPTIONS].add_texture(main_text_font, "Options", rgba, x_coord, y_coord);
	menu_objects[MENU_GROUP_RENDER_ALWAYS][SUB_MENU_EXTRAS].add_texture(main_text_font, "Extras", rgba, x_coord, y_coord);

	//Add the background textures to the "only render while active" group

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].add_texture("resource/ui/menu/main/missingno.png");
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].add_texture("resource/ui/menu/main/missingno.png");
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].add_texture("resource/ui/menu/main/vsimg.png");
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].add_texture("resource/ui/menu/main/missingno.png");
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].add_texture("resource/ui/menu/main/missingno.png");

	for (int i = 0; i < 5; i++) { //Quick initialization things
		GameTexture* bg_texture = &menu_objects[MENU_GROUP_RENDER_ACTIVE][i].textures.back();
		bg_texture->set_width_scale(1.5);
		bg_texture->set_height_scale(1.5);
		bg_texture->set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);

		GameTexture& cursor = menu_objects[MENU_GROUP_RENDER_ACTIVE][i].cursor;
		cursor.init("resource/ui/menu/main/Cursor.png");
		cursor.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		cursor.set_pos(glm::vec3(700.0, 0.0, 0.0));
		cursor.set_width(50);
		cursor.set_height(50);
	}

	//Add a new child for each sub option. At the moment it'd probably be better to add new textures instead
	//of new children, but their behavior is going to become more complex later so it makes sense.

	for (int i = 0; i < 3; i++) {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].add_child(false);
		menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].add_child(false);
		menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].add_child(false);
	}

	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].add_child(false);
	}

	for (int i = 0; i < 4; i++) {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].add_child(false);
	}

	//Admittedly the deeper down the tree we go, the more ridiculous this code starts to look. This section
	//is creating the text for each sub menu object. Later on, we'll also add a description text to render
	//when you're hovering over a sub menu option.

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_LOBBY].add_texture(
		sub_text_font, "Lobby", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_QUEUE].add_texture(
		sub_text_font, "Queue", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_COACH].add_texture(
		sub_text_font, "Coach", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_STORY].add_texture(
		sub_text_font, "Story", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_ARCADE].add_texture(
		sub_text_font, "Arcade", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_TRAINING].add_texture(
		sub_text_font, "Training", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_PVP].add_texture(
		sub_text_font, "Player Vs. Player", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_PVC].add_texture(
		sub_text_font, "Player Vs. CPU", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_TOURNAMENT].add_texture(
		sub_text_font, "Tournament", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_CONTROLS].add_texture(
		sub_text_font, "Controls", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_GRAPHICS].add_texture(
		sub_text_font, "Graphics", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_AUDIO].add_texture(
		sub_text_font, "Audio", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[3].add_texture(
		sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[4].add_texture(
		sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[SUB_EXTRAS_SOUND_TEST].add_texture(
		sub_text_font, "Sound Test", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[SUB_EXTRAS_GALLERY].add_texture(
		sub_text_font, "Gallery", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[2].add_texture(
		sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[3].add_texture(
		sub_text_font, "Placeholder", glm::vec4(255.0, 127.0, 0.0, 255.0), 12.0, -12.0
	);

	//All top level menus have identical behavior so we can just use a loop to define their events

	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][i].up_event = [](MenuObject* menu_object) {
			menu_object->active_child--;
			if (menu_object->active_child == -1) {
				menu_object->active_child += menu_object->children.size();
			}
		};
		menu_objects[MENU_GROUP_RENDER_ACTIVE][i].down_event = [](MenuObject* menu_object) {
			menu_object->active_child++;
			if (menu_object->active_child == menu_object->children.size()) {
				menu_object->active_child = 0;
			}
		};
		menu_objects[MENU_GROUP_RENDER_ACTIVE][i].select_event = [](MenuObject* menu_object) {
			menu_object->children[menu_object->active_child].event_select_press();
		};
		menu_objects[MENU_GROUP_RENDER_ACTIVE][i].process_event = [this](MenuObject* menu_object) {
			menu_object->cursor.set_pos(glm::vec3(table.pos.x + 80, menu_object->children[menu_object->active_child].textures[0].pos.y - WINDOW_HEIGHT * 1.125, 0));
		};

		//Also we left-orient all of the sub menu text

		for (int i2 = 0, max2 = menu_objects[MENU_GROUP_RENDER_ACTIVE][i].children.size(); i2 < max2; i2++) {
			menu_objects[MENU_GROUP_RENDER_ACTIVE][i].children[i2].textures[0].set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		}
	}

	//now we create the events for each menu option. for now these just set the state to update to but
	//things will get weird when we set up the options menu

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_LOBBY].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_QUEUE].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_ONLINE].children[SUB_ONLINE_COACH].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_STORY].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_ARCADE].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_SOLO].children[SUB_SOLO_TRAINING].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_CHARA_SELECT, GAME_CONTEXT_TRAINING);
		*this->looping = false;
	};

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_PVP].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_CHARA_SELECT, GAME_CONTEXT_NORMAL);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_PVC].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_CHARA_SELECT, GAME_CONTEXT_1CPU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_VS].children[SUB_VS_TOURNAMENT].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_CONTROLS].select_event = [this](MenuObject* menu_object) {

	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_GRAPHICS].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[SUB_OPTIONS_AUDIO].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[3].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_OPTIONS].children[4].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};

	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[SUB_EXTRAS_SOUND_TEST].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[SUB_EXTRAS_GALLERY].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[2].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};
	menu_objects[MENU_GROUP_RENDER_ACTIVE][SUB_MENU_EXTRAS].children[3].select_event = [this](MenuObject* menu_object) {
		this->update_state(GAME_STATE_DEBUG_MENU);
		*this->looping = false;
	};

	table.init("resource/ui/menu/main/SubMenu.png");
	table.set_orientation(GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT);
	table.set_pos(glm::vec3(WINDOW_WIDTH * 2, 0.0, 0.0));
	table.scale_top_percent(1.2, false);
	table.scale_bottom_percent(1.2, false);

	background_texture.init("resource/ui/menu/main/bg.png");
	background_texture.load_spritesheet("resource/ui/menu/main/bg.yml");

	inc_thread();
	game_loader->finished = true;

	main_text_font.unload_font();
	sub_text_font.unload_font();
}

void MainMenu::process_main() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (menu_frame > 0) {
			menu_frame--;
		}
	}
	else {
		if (menu_frame < 1) { //TODO: When a full spritesheet is implemented, replace 1 w the number of frames
			menu_frame++;
		}
	}
	background_texture.set_sprite(menu_frame);

	table.process();
	for (int i = 0; i < 5; i++) {
		for (int i2 = 0, max2 = menu_objects[MENU_GROUP_RENDER_ACTIVE][i].children.size(); i2 < max2; i2++) {
			menu_objects[MENU_GROUP_RENDER_ACTIVE][i].children[i2].textures[0].set_pos(glm::vec3(table.pos.x + 160, (float)WINDOW_HEIGHT * 0.5 + (i2 * 130), 0.0));
		}
		menu_objects[MENU_GROUP_RENDER_ACTIVE][i].process();
	}
}

void MainMenu::render() {
	background_texture.render();

	//prebuffer render
	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_pos(glm::vec3(int(magnitude * cos(theta + (i - 5) * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + (i - 5) * offset)), 0.0));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_rot(glm::vec3(0.0, 0.0, ((theta + (i - 5) * offset) * 180) / 3.14));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].render();

	}

	//real render
	table.render();
	menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].cursor.render();
	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_pos(glm::vec3(int(magnitude * cos(theta + i * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + i * offset)), 0.0));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_rot(glm::vec3(0.0, 0.0, ((theta + i * offset) * 180) / 3.14));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].render();


		for (int i2 = 0, max2 = menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].children.size(); i2 < max2; i2++) {
			menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].children[i2].render();
		}
	}

	//postbuffer render
	for (int i = 0; i < 5; i++) {
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_pos(glm::vec3(int(magnitude * cos(theta + (i + 5) * offset)) - WINDOW_WIDTH / 1.367, int(magnitude * sin(theta + (i + 5) * offset)), 0.0));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].set_rot(glm::vec3(0.0, 0.0, ((theta + (i + 5) * offset) * 180) / 3.14));
		menu_objects[MENU_GROUP_RENDER_ALWAYS][i].textures[0].render();
	}

	menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].textures[0].render();

	theta += ((top_selection * -offset) - theta) / turn_frames;
}

void MainMenu::process_background() {
	process_main();
	render();
}

void MainMenu::event_up_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection < 4) {
			top_selection++;
		}
		else {
			top_selection = 0;
			theta += 5 * offset;
		}
	}
	else {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].event_up_press();
	}
}

void MainMenu::event_down_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		if (top_selection > 0) {
			top_selection--;
		}
		else {
			top_selection = 4;
			theta -= 5 * offset;
		}
	}
	else {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].event_down_press();
	}
}

void MainMenu::event_left_press() {
	if (menu_level == MENU_LEVEL_SUB) {
		event_back_press();
	}
}

void MainMenu::event_right_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		event_select_press();
	}
}

void MainMenu::event_start_press() {
	*game_state = GAME_STATE_DEBUG_MENU;
	*looping = false;
}

void MainMenu::event_select_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		table.set_target_pos(glm::vec3(WINDOW_WIDTH * 1.535, 0.0, 0.0), 6);
		menu_level = MENU_LEVEL_SUB;
	}
	else {
		menu_objects[MENU_GROUP_RENDER_ACTIVE][top_selection].event_select_press();
	}
}

void MainMenu::event_back_press() {
	if (menu_level == MENU_LEVEL_TOP) {
		update_state(GAME_STATE_TITLE_SCREEN);
		*looping = false;
	}
	else {
		table.set_target_pos(glm::vec3(WINDOW_WIDTH * 2, 0.0, 0.0), 6);
		menu_level = MENU_LEVEL_TOP;
	}
}