#include "Battle.h"

TrainingInfo::TrainingInfo() {
}

void TrainingInfo::init(Fighter* fighter, Font& font) {
	background_texture.init("resource/game_state/battle/ui/training/training_info.png");
	hit_frame.init(font, "Hit Frame: ", glm::vec4(255.0));
	damage.init(font, "Damage: ", glm::vec4(255.0));
	combo_damage.init(font, "Total: ", glm::vec4(255.0));
	stun_frames.init(font, "Stun Frames: ", glm::vec4(255.0));
	frame_advantage.init(font, "Frame Advantage: ", glm::vec4(255.0));

	hit_frame.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	damage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	combo_damage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	stun_frames.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	frame_advantage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);

	background_texture.set_pos(glm::vec3(250.0, 150.0, 0.0));
	background_texture.set_scale(0.7);
	if (fighter->id) {
		background_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		hit_frame.set_pos(glm::vec3(2810.0, 320.0, 0.0));
		damage.set_pos(glm::vec3(2810.0, 390.0, 0.0));
		combo_damage.set_pos(glm::vec3(2810.0, 460.0, 0.0));
		stun_frames.set_pos(glm::vec3(2810.0, 530.0, 0.0));
		frame_advantage.set_pos(glm::vec3(2810.0, 600.0, 0.0));
	}
	else {
		background_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		hit_frame.set_pos(glm::vec3(370.0, 320.0, 0.0));
		damage.set_pos(glm::vec3(370.0, 390.0, 0.0));
		combo_damage.set_pos(glm::vec3(370.0, 460.0, 0.0));
		stun_frames.set_pos(glm::vec3(370.0, 530.0, 0.0));
		frame_advantage.set_pos(glm::vec3(370.0, 600.0, 0.0));
	}
}

void TrainingInfo::render() {
	background_texture.render();
	hit_frame.render();
	damage.render();
	combo_damage.render();
	stun_frames.render();
	frame_advantage.render();
	input_visualizer.render();
	for (int i = 0; i < 7; i++) {
		buttons[i].render();
	}
}