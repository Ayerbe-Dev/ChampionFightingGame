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
			buttons[i].set_pos(glm::vec3(925.0, 0.0, 0.0));
		}
		for (int i = 0; i < 9; i++) {
			stick[i].init("resource/game_state/battle/ui/training/" + std::to_string(i + 1) + ".png");
			stick[i].set_pos(glm::vec3(925.0, 0.0, 0.0));
		}
		background.set_pos(glm::vec3(925.0, 0.0, 0.0));
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

	input_visualizer.init(fighter, font, false);

	hit_frame.init(*font, "Hit Frame: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	damage.init(*font, "Damage: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	combo_damage.init(*font, "Total: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	stun_frames.init(*font, "Stun Frames: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	frame_advantage.init(*font, "Frame Advantage: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));

	hit_frame.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	damage.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	combo_damage.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	stun_frames.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
	frame_advantage.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);

	background_texture.set_pos(glm::vec3(350.0, 150.0, 0.0));
	background_texture.set_scale(0.7);
	hit_frame.set_scale(0.8);
	damage.set_scale(0.8);
	combo_damage.set_scale(0.8);
	stun_frames.set_scale(0.8);
	frame_advantage.set_scale(0.8);
	if (fighter->id) {
		background_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_RIGHT);
		hit_frame.set_pos(glm::vec3(2680.0, 280.0, 0.0));
		damage.set_pos(glm::vec3(2680.0, 350.0, 0.0));
		combo_damage.set_pos(glm::vec3(2680.0, 420.0, 0.0));
		stun_frames.set_pos(glm::vec3(2680.0, 490.0, 0.0));
		frame_advantage.set_pos(glm::vec3(2680.0, 560.0, 0.0));
	}
	else {
		background_texture.set_orientation(SCREEN_TEXTURE_ORIENTATION_TOP_LEFT);
		hit_frame.set_pos(glm::vec3(440.0, 280.0, 0.0));
		damage.set_pos(glm::vec3(440.0, 350.0, 0.0));
		combo_damage.set_pos(glm::vec3(440.0, 420.0, 0.0));
		stun_frames.set_pos(glm::vec3(440.0, 490.0, 0.0));
		frame_advantage.set_pos(glm::vec3(440.0, 560.0, 0.0));
	}
	mini_visualizers.resize(20);
}

void TrainingInfo::destroy() {
	background_texture.destroy();
	hit_frame.destroy();
	damage.destroy();
	combo_damage.destroy();
	stun_frames.destroy();
	frame_advantage.destroy();
}

void TrainingInfo::render() {
	background_texture.render();
	hit_frame.render();
	damage.render();
	combo_damage.render();
	stun_frames.render();
	frame_advantage.render();
	input_visualizer.render();
	for (std::list<InputVisualizer>::iterator it = mini_visualizers.begin(),
		max = mini_visualizers.end(); it != max; it++) {
		it->render();
	}
}