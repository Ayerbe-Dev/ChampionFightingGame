#include "Battle.h"
#include "utils.h"

InputVisualizer::InputVisualizer() {
	fighter = nullptr;
	font = nullptr;
	frame_timer = 0;
	keep_frames = false;
	input_code = 0;
}

void InputVisualizer::init(Fighter* fighter, Font* font, bool keep_frames) {
	this->fighter = fighter;
	this->font = font;
	this->keep_frames = keep_frames;
	background.init("resource/game_state/battle/ui/training/visualizer.png");

	buttons[0].init("resource/game_state/battle/ui/training/lp.png");
	buttons[1].init("resource/game_state/battle/ui/training/mp.png");
	buttons[2].init("resource/game_state/battle/ui/training/hp.png");
	buttons[3].init("resource/game_state/battle/ui/training/lk.png");
	buttons[4].init("resource/game_state/battle/ui/training/mk.png");
	buttons[5].init("resource/game_state/battle/ui/training/hk.png");
	if (keep_frames) {
		float x = 30.0;
		if (fighter->id) {
			x = 3540.0;
		}
		for (int i = 0; i < 6; i++) {
			buttons[i].set_pos(glm::vec3(x, 50.0, 0.0));
			buttons[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
			buttons[i].set_scale(0.2);
		}
		for (int i = 0; i < 9; i++) {
			stick[i].init("resource/game_state/battle/ui/training/" + std::to_string(i + 1) + ".png");
			stick[i].set_pos(glm::vec3(x, 50.0, 0.0));
			stick[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
			stick[i].set_scale(0.2);
		}
		background.set_pos(glm::vec3(x, 50.0, 0.0));
		background.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
		background.set_scale(0.2);
		frame_timer = 1;
		num_frames.init(*font, "1", glm::vec4(255.0),
			glm::vec4(0.0, 0.0, 0.0, 1.0)
		);
		num_frames.set_pos(glm::vec3(x + 200.0, 50.0, 0.0));
		num_frames.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);


	}
	else {
		for (int i = 0; i < 6; i++) {
			buttons[i].set_pos(glm::vec3(1150.0, 0.0, 0.0));
			buttons[i].set_scale(0.8);
		}
		for (int i = 0; i < 9; i++) {
			stick[i].init("resource/game_state/battle/ui/training/" + std::to_string(i + 1) + ".png");
			stick[i].set_pos(glm::vec3(1150.0, 0.0, 0.0));
			stick[i].set_scale(0.8);
		}
		background.set_pos(glm::vec3(1150.0, 0.0, 0.0));
		background.set_scale(0.8);
		if (fighter->id) {
			background.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
			for (int i = 0; i < 9; i++) {
				stick[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
			}
			for (int i = 0; i < 6; i++) {
				buttons[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
			}
		}
		else {
			background.set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
			for (int i = 0; i < 9; i++) {
				stick[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
			}
			for (int i = 0; i < 6; i++) {
				buttons[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_BOTTOM_LEFT);
			}
		}
	}
}

void InputVisualizer::render() {
	if (fighter == nullptr) {
		return;
	}
	background.render();
	if (input_code == 0) {
		for (int i = 0; i < 6; i++) {
			if (fighter->check_button_on(BUTTON_LP + i)) {
				buttons[i].render();
			}
		}
		stick[fighter->get_stick_dir_no_lr() - 1].render();
	}
	else {
		for (int i = 0; i < 6; i++) {
			if (input_code & (1 << i)) {
				buttons[i].render();
			}
		}
		stick[(input_code >> 7) - 1].render();
		num_frames.render();
	}
}

TrainingInfo::TrainingInfo() {
	fighter = nullptr;
}

void TrainingInfo::init(Fighter* fighter, Font* font) {
	this->fighter = fighter;
	background_texture.init("resource/game_state/battle/ui/training/training_info.png");
	background_texture.set_pos(glm::vec3(350.0, 150.0, 0.0));
	background_texture.set_scale(0.7);

	input_visualizer.init(fighter, font, false);

	for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
		fields[i].init(*font, field_names[i], glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
		fields[i].set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
		fields[i].set_scale(0.8);
	}

	if (fighter->id) {
		background_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
		for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
			fields[i].set_pos(glm::vec3(2560.0, 210 + 70 * i, 0.0));
		}
	}
	else {
		background_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
		for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
			fields[i].set_pos(glm::vec3(460.0, 210 + 70 * i, 0.0));
		}
	}
	mini_visualizers.resize(20);
}

void TrainingInfo::destroy() {
	background_texture.destroy();
	for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
		fields[i].destroy();
	}
}

void TrainingInfo::render() {
	background_texture.render();
	for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
		fields[i].render();
	}
	input_visualizer.render();
	for (std::list<InputVisualizer>::iterator it = mini_visualizers.begin(),
		max = mini_visualizers.end(); it != max; it++) {
		it->render();
	}
}