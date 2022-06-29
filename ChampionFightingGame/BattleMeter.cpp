#include "Battle.h"
#include "ParamAccessor.h"

BattleMeter::BattleMeter() {
	health = nullptr;
	max_health = 0.0;
	ex = nullptr;
	max_ex = 0.0;
	num_bars = 0;
}

void BattleMeter::init(Fighter* fighter) {
	health_texture.init("resource/ui/battle/hp/health.png");
	health_border.init("resource/ui/battle/hp/bar.png");
	ex_texture.init("resource/ui/battle/ex/ex.png");
	ex_segment_texture.init("resource/ui/battle/ex/ex_segment.png");
	ex_border.init("resource/ui/battle/ex/bar.png");

	if (fighter->id) {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		health_border.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_border.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);

		health_texture.flip_h();
		health_border.flip_h();
		ex_texture.flip_h();
		ex_segment_texture.flip_h();
		ex_border.flip_h();
	}
	else {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		health_border.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_border.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	health_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	health_border.set_pos(glm::vec3(50.0, 57.0, 0.0));
	ex_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));
	ex_segment_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));

	health = &fighter->fighter_float[FIGHTER_FLOAT_HEALTH];
	max_health = fighter->get_local_param_float("health");
	ex = &fighter->fighter_float[FIGHTER_FLOAT_SUPER_METER];
	max_ex = get_param_int("ex_meter_size", PARAM_FIGHTER);
	num_bars = get_param_int("ex_meter_bars", PARAM_FIGHTER);

	ex_texture.scale_right_percent(0.0);
	ex_segment_texture.scale_right_percent(0.0);
}

void BattleMeter::destroy() {
	health_texture.destroy();
	health_border.destroy();
	ex_texture.destroy();
	ex_segment_texture.destroy();
	ex_border.destroy();
}

void BattleMeter::process() {
	health_texture.scale_left_percent(*health / max_health);
	health_texture.prepare_render();
	health_border.prepare_render();

	ex_texture.set_right_target(*ex / max_ex, 6);

	int segments = floor(*ex / (max_ex / num_bars));
	if (prev_segments != segments) {
		ex_segment_texture.set_right_target(clampf(0.0, (float)segments / num_bars, 1.0), 1);
	}
	prev_segments = segments;
	ex_texture.process();
	ex_segment_texture.process();
	ex_texture.prepare_render();
	ex_segment_texture.prepare_render();
	ex_border.prepare_render();
}

void BattleMeter::render() {
	health_texture.render_prepared();
	health_border.render_prepared();
	ex_texture.render_prepared();
	ex_segment_texture.render_prepared();
	ex_border.render_prepared();
}