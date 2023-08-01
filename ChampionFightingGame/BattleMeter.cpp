#include "Battle.h"
#include "ParamAccessor.h"
#include "utils.h"
#include "ShaderManager.h"

BattleMeter::BattleMeter() {
	health = nullptr;
	partial_health = nullptr;
	ended_hitstun = nullptr;
	disable_hitstun_parry = nullptr;
	max_health = 0.0;
	ex = nullptr;
	max_ex = 0.0;
	num_bars = 0;
	prev_segments = 0.0;
	wins = 0;
}

void BattleMeter::init(Fighter* fighter, int num_rounds) {
	health_texture.init("resource/game_state/battle/ui/hp/health.png");
	partial_health_texture.init("resource/game_state/battle/ui/hp/health.png");
	combo_health_texture.init("resource/game_state/battle/ui/hp/combo_health.png");
	combo_health_texture.attach_shader(ShaderManager::get_instance()->get_shader("2d_texture", "2d_texture", "", SHADER_FEAT_COLORMOD));
	health_border.init("resource/game_state/battle/ui/hp/bar.png");
	ex_texture.init("resource/game_state/battle/ui/ex/ex.png");
	ex_segment_texture.init("resource/game_state/battle/ui/ex/ex_segment.png");
	ex_border.init("resource/game_state/battle/ui/ex/bar.png");

	if (fighter->id) {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		partial_health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		combo_health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		health_border.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_border.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);

		health_texture.flip_h();
		partial_health_texture.flip_h();
		combo_health_texture.flip_h();
		health_border.flip_h();
		ex_texture.flip_h();
		ex_segment_texture.flip_h();
		ex_border.flip_h();
	}
	else {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		partial_health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		combo_health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		health_border.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_border.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	health_texture.set_alpha(127);
	health_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	partial_health_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	combo_health_texture.set_pos(glm::vec3(50.0, 57.0, 0.0));
	health_border.set_pos(glm::vec3(50.0, 57.0, 0.0));
	ex_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));
	ex_segment_texture.set_pos(glm::vec3(119.0, 60.0, 0.0));

	health = &fighter->fighter_float[FIGHTER_FLOAT_HEALTH];
	partial_health = &fighter->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
	ended_hitstun = fighter->fighter_flag[FIGHTER_FLAG_ENDED_HITSTUN]._Getptr();
	disable_hitstun_parry = fighter->fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY]._Getptr();
	max_health = fighter->get_local_param_float("health");
	ex = &fighter->fighter_float[FIGHTER_FLOAT_SUPER_METER];
	max_ex = (float)get_param_int(PARAM_FIGHTER, "ex_meter_size");
	num_bars = get_param_int(PARAM_FIGHTER, "ex_meter_bars");
	ex_texture.scale_right_percent(0.0);
	ex_segment_texture.scale_right_percent(0.0);
	for (int i = 0; i < num_rounds; i++) {
		
	}
}

void BattleMeter::destroy() {
	health_texture.destroy();
	partial_health_texture.destroy();
	combo_health_texture.destroy();
	health_border.destroy();
	ex_texture.destroy();
	ex_segment_texture.destroy();
	ex_border.destroy();
}

void BattleMeter::process() {
	if (*partial_health < *health) {
		combo_health_texture.scale_left_percent(*partial_health / max_health);
	}
	if (*ended_hitstun) {
		combo_health_texture.set_left_target(clampf(0.0, *health / max_health, 1.0), 5.0);
	}
	if (*disable_hitstun_parry) {
		combo_health_texture.set_colormod(glm::vec3(-30.0, 0.0, 40.0));
	}
	else {
		combo_health_texture.set_colormod(glm::vec3(0.0));
	}
	combo_health_texture.process();
	health_texture.scale_left_percent(*health / max_health);
	partial_health_texture.scale_left_percent(*partial_health / max_health);
	health_texture.prepare_render();
	partial_health_texture.prepare_render();
	combo_health_texture.prepare_render();
	health_border.prepare_render();

	ex_texture.set_right_target(*ex / max_ex, 6);


	float segments = floor(*ex / (max_ex / num_bars));
	if (prev_segments != segments) {
		ex_segment_texture.set_right_target(clampf(0.0, segments / (float)num_bars, 1.0), 1);
	}
	prev_segments = segments;
	ex_texture.process();
	ex_segment_texture.process();
	ex_texture.prepare_render();
	ex_segment_texture.prepare_render();
	ex_border.prepare_render();
}

void BattleMeter::render() {
	combo_health_texture.render_prepared();
	health_texture.render_prepared();
	partial_health_texture.render_prepared();
	health_border.render_prepared();
	ex_texture.render_prepared();
	ex_segment_texture.render_prepared();
	ex_border.render_prepared();
}