#include "Battle.h"

TrainingInfo::TrainingInfo() {
}

void TrainingInfo::init(Fighter* fighter, Font& font) {
	background_texture.init("resource/game_state/battle/ui/training/training_info.png");
	hit_frame.init(font, "Hit Frame: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	damage.init(font, "Damage: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	combo_damage.init(font, "Total: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	stun_frames.init(font, "Stun Frames: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));
	frame_advantage.init(font, "Frame Advantage: ", glm::vec4(255.0), glm::vec4(0.0, 0.0, 0.0, 2.0));

	hit_frame.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	damage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	combo_damage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	stun_frames.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	frame_advantage.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);

	background_texture.set_pos(glm::vec3(250.0, 150.0, 0.0));
	background_texture.set_scale(0.7);
	if (fighter->id) {
		background_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		hit_frame.set_pos(glm::vec3(2810.0, 280.0, 0.0));
		damage.set_pos(glm::vec3(2810.0, 350.0, 0.0));
		combo_damage.set_pos(glm::vec3(2810.0, 420.0, 0.0));
		stun_frames.set_pos(glm::vec3(2810.0, 490.0, 0.0));
		frame_advantage.set_pos(glm::vec3(2810.0, 560.0, 0.0));
	}
	else {
		background_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		hit_frame.set_pos(glm::vec3(370.0, 280.0, 0.0));
		damage.set_pos(glm::vec3(370.0, 350.0, 0.0));
		combo_damage.set_pos(glm::vec3(370.0, 420.0, 0.0));
		stun_frames.set_pos(glm::vec3(370.0, 490.0, 0.0));
		frame_advantage.set_pos(glm::vec3(370.0, 560.0, 0.0));
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