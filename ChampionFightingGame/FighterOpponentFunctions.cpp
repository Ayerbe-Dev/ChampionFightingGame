#include "Fighter.h"

void Fighter::set_opponent_offset(vec2 offset, int frames) {
	vec2 new_offset;
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES] = frames + 1;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::set_opponent_offset(vec2 offset) {
	vec2 new_offset;
	int frames = fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES];
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::change_opponent_status(u32 status_kind) {
	if ((this->status_kind == FIGHTER_STATUS_THROW || this->status_kind == FIGHTER_STATUS_THROW_AIR) && status_kind == FIGHTER_STATUS_THROWN) {
		fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = true;
	}
	fighter_accessor->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage, float facing_dir, float x_speed, float y_speed) {
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_HEALTH] -= damage;
	fighter_accessor->fighter[!id]->facing_dir = facing_dir * this->facing_dir * -1;
	fighter_accessor->fighter[!id]->facing_right = (facing_dir * this->facing_dir * -1) > 0;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = x_speed * this->facing_dir;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = y_speed;
}

void Fighter::set_opponent_rot(vec3 rot) {
	fighter_accessor->fighter[!id]->set_rot(rot * facing_dir);
}

void Fighter::add_opponent_rot(vec3 rot) {
	fighter_accessor->fighter[!id]->add_rot(rot * facing_dir);
}

void Fighter::reset_opponent_rot() {
	fighter_accessor->fighter[!id]->reset_rot();
}

void Fighter::set_opponent_thrown_ticks() {
	if (fighter_accessor->fighter[!id]->anim_kind == nullptr) {
		return;
	}
	fighter_accessor->fighter[!id]->rate = (((anim_kind->length - 1) / this->rate) / (fighter_accessor->fighter[!id]->anim_kind->length - 1)) - 1;
}

void Fighter::change_opponent_anim(string anim_kind, float frame_rate, float entry_frame) {
	fighter_accessor->fighter[!id]->change_anim(anim_kind, frame_rate, entry_frame);
}