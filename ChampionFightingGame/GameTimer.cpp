#include "Battle.h"

GameTimer::GameTimer() {
	clock_mode = 0;
	deca_frames = 0;
	frames = 0;
	deca_seconds = 0;
	seconds = 0;
	time_up = false;
}

GameTimer::GameTimer(int time) {
	init(time);
};

void GameTimer::init(int time) {
	max_time = time;
	if (time == -1) {
		deca_seconds = 9;
		seconds = 9;
		deca_frames = 5;
		frames = 9;
		clock_mode = 1;
	}
	else {
		deca_seconds = time / 10;
		seconds = time % 10;
		deca_frames = 5;
		frames = 9;
		clock_mode = 1;
	}

	clock.init("resource/game_state/battle/ui/timer/clockface.png");
	second_texture.init("resource/game_state/battle/ui/timer/bigtypeface.png");
	deca_second_texture.init("resource/game_state/battle/ui/timer/bigtypeface.png");
	frame_texture.init("resource/game_state/battle/ui/timer/smalltypeface.png");
	deca_frame_texture.init("resource/game_state/battle/ui/timer/smalltypeface.png");

	clock.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	second_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	deca_second_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	frame_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);
	deca_frame_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_MIDDLE);

	clock.load_spritesheet("resource/game_state/battle/ui/timer/clockface.yml");
	second_texture.load_spritesheet("resource/game_state/battle/ui/timer/bigtypeface.yml");
	deca_second_texture.load_spritesheet("resource/game_state/battle/ui/timer/bigtypeface.yml");
	frame_texture.load_spritesheet("resource/game_state/battle/ui/timer/smalltypeface.yml");
	deca_frame_texture.load_spritesheet("resource/game_state/battle/ui/timer/smalltypeface.yml");

	clock.set_scale(1.5);
	deca_second_texture.set_scale(1.5);
	second_texture.set_scale(1.5);
	deca_frame_texture.set_scale(1.5);
	frame_texture.set_scale(1.5);

	clock.set_pos(glm::vec3(0.0, 20.0, 0.0));
	deca_second_texture.set_pos(glm::vec3(-76.0, 38.5, 0.0));
	second_texture.set_pos(glm::vec3(4.0, 38.5, 0.0));
	deca_frame_texture.set_pos(glm::vec3(68.0, 155.0, 0.0));
	frame_texture.set_pos(glm::vec3(100.0, 155.0, 0.0));

	clock.set_sprite(1);
}

void GameTimer::reset() {
	deca_seconds = max_time / 10;
	seconds = max_time % 10;
	deca_frames = 5;
	frames = 9;
	clock_mode = 1;
	time_up = false;
}

void GameTimer::process() {
	if (!BattleObjectManager::get_instance()->frame_elapsed()) {
		return;
	}
	if (frames == 0) {
		if (deca_frames == 0) {
			if (seconds == 0 && deca_seconds == 0) {
				time_up = true;
			}
			else {
				deca_frames = 5;
				frames = 9;
				if (max_time != -1) {
					if (seconds == 0) {
						deca_seconds--;
						seconds = 9;
					}
					else {
						seconds--;
					}

				}			
			}
		}
		else {
			deca_frames--;
			frames = 9;
		}
	}
	else {
		frames--;
	}
	second_texture.set_sprite(seconds);
	deca_second_texture.set_sprite(deca_seconds);
	frame_texture.set_sprite(frames);
	deca_frame_texture.set_sprite(deca_frames);

	clock.prepare_render();
	second_texture.prepare_render();
	deca_second_texture.prepare_render();
	frame_texture.prepare_render();
	deca_frame_texture.prepare_render();
}

void GameTimer::flip_clock() {
	clock_mode = !clock_mode;
	clock.set_sprite(clock_mode);
}

void GameTimer::render() {
	clock.render_prepared();
	second_texture.render_prepared();
	deca_second_texture.render_prepared();
	frame_texture.render_prepared();
	deca_frame_texture.render_prepared();
}