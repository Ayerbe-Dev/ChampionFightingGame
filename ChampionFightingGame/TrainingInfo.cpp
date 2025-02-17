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
	background.init("resource/scene/battle/ui/training/visualizer.png");

	buttons[0].init("resource/scene/battle/ui/training/lp.png");
	buttons[1].init("resource/scene/battle/ui/training/mp.png");
	buttons[2].init("resource/scene/battle/ui/training/hp.png");
	buttons[3].init("resource/scene/battle/ui/training/lk.png");
	buttons[4].init("resource/scene/battle/ui/training/mk.png");
	buttons[5].init("resource/scene/battle/ui/training/hk.png");
	if (keep_frames) {
		float x = 30.0;
		if (fighter->id) {
			x = 3540.0;
		}
		for (int i = 0; i < 6; i++) {
			buttons[i]
				.set_pos(glm::vec3(x, 50.0, 0.0))
				.set_orientation(TEXTURE_TOP_LEFT)
				.set_scale(0.2)
				.set_alpha(0);
		}
		for (int i = 0; i < 9; i++) {
			stick[i]
				.init("resource/scene/battle/ui/training/" + std::to_string(i + 1) + ".png")
				.set_pos(glm::vec3(x, 50.0, 0.0))
				.set_orientation(TEXTURE_TOP_LEFT)
				.set_scale(0.2)
				.set_alpha(0);
		}
		background
			.set_pos(glm::vec3(x, 50.0, 0.0))
			.set_orientation(TEXTURE_TOP_LEFT)
			.set_scale(0.2f)
			.set_alpha(0);
		frame_timer = 1;
		num_frames
			.init(font, "1", TextSpecifier().border(1))
			.set_pos(glm::vec3(x + 200.0, 50.0, 0.0))
			.set_orientation(TEXTURE_TOP_LEFT)
			.set_alpha(0);
	}
	else {
		for (int i = 0; i < 6; i++) {
			buttons[i]
				.set_pos(glm::vec3(1150.0, 0.0, 0.0))
				.set_scale(0.8);
		}
		for (int i = 0; i < 9; i++) {
			stick[i]
				.init("resource/scene/battle/ui/training/" + std::to_string(i + 1) + ".png")
				.set_pos(glm::vec3(1150.0, 0.0, 0.0))
				.set_scale(0.8);
		}
		background
			.set_pos(glm::vec3(1150.0, 0.0, 0.0))
			.set_scale(0.8);
		if (fighter->id) {
			background.set_orientation(TEXTURE_BOTTOM_RIGHT);
			for (int i = 0; i < 9; i++) {
				stick[i].set_orientation(TEXTURE_BOTTOM_RIGHT);
			}
			for (int i = 0; i < 6; i++) {
				buttons[i].set_orientation(TEXTURE_BOTTOM_RIGHT);
			}
		}
		else {
			background.set_orientation(TEXTURE_BOTTOM_LEFT);
			for (int i = 0; i < 9; i++) {
				stick[i].set_orientation(TEXTURE_BOTTOM_LEFT);
			}
			for (int i = 0; i < 6; i++) {
				buttons[i].set_orientation(TEXTURE_BOTTOM_LEFT);
			}
		}
	}
}

void InputVisualizer::reset() {
	float x = 30.0;
	if (fighter->id) {
		x = 3540.0;
	}
	for (int i = 0; i < 6; i++) {
		buttons[i]
			.set_pos(glm::vec3(x, 50.0, 0.0))
			.set_alpha(255);
	}
	for (int i = 0; i < 9; i++) {
		stick[i]
			.set_pos(glm::vec3(x, 50.0, 0.0))
			.set_alpha(255);
	}
	background
		.set_pos(glm::vec3(x, 50.0, 0.0))
		.set_alpha(255);
	frame_timer = 1;
	num_frames
		.set_pos(glm::vec3(x + 200.0, 50.0, 0.0))
		.set_alpha(255);
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
	background_texture
		.init("resource/scene/battle/ui/training/training_info.png")
		.set_pos(glm::vec3(350.0, 150.0, 0.0))
		.set_scale(0.7);

	input_visualizer.init(fighter, font, false);

	for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
		fields[i]
			.init(font, field_names[i], TextSpecifier().border(2))
			.set_orientation(TEXTURE_TOP_LEFT)
			.set_scale(0.8);
	}

	if (fighter->id) {
		background_texture.set_orientation(TEXTURE_TOP_RIGHT);
		for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
			fields[i].set_pos(glm::vec3(2560.0, 210 + 70 * i, 0.0));
		}
	}
	else {
		background_texture.set_orientation(TEXTURE_TOP_LEFT);
		for (int i = 0; i < TRAINING_FIELD_MAX; i++) {
			fields[i].set_pos(glm::vec3(460.0, 210 + 70 * i, 0.0));
		}
	}
	mini_visualizers.resize(20);
	for (int i = 0; i < 20; i++) {
		mini_visualizers.newest(i).init(fighter, font, true);
	}
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
	for (int i = 0; i < mini_visualizers.size(); i++) {
		mini_visualizers.newest(i).render();
	}
}