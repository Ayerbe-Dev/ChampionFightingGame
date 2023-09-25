#include "Fighter.h"
#include "Projectile.h"
#include "utils.h"

void Fighter::process_fighter_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes) {
	//This function only runs for P1
	Fighter* that = battle_object_manager->fighter[1];
	float x_diff = fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED];
	float that_x_diff = that->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED];
	bool allow_crossup = fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP];
	bool that_allow_crossup = that->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP];
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	that->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	if (pushboxes_touching(that)) {
		if (pos.x == that->pos.x) {
			if ((status_kind == FIGHTER_STATUS_JUMP 
				|| status_kind == FIGHTER_STATUS_FALL) && x_diff != 0.0
				&& (x_diff > 0.0 == facing_dir > 0.0)) {
				float walk_f_speed = that->get_local_param_float("walk_f_speed") * 2;
				if (x_diff > 0.0) {
					that->pos.x -= walk_f_speed;
				}
				else {
					that->pos.x += walk_f_speed;
				}
				that->update_pushbox_pos();
			}
			if ((that->status_kind == FIGHTER_STATUS_JUMP 
				|| that->status_kind == FIGHTER_STATUS_FALL) && that_x_diff != 0.0
				&& (that_x_diff > 0.0 == that->facing_dir > 0.0)) {
				float walk_f_speed = get_local_param_float("walk_f_speed") * 2;
				if (that_x_diff > 0.0) {
					pos.x -= walk_f_speed;
				}
				else {
					pos.x += walk_f_speed;
				}
				update_pushbox_pos();
			}
		}
	}
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		for (size_t i2 = 0, max2 = that_pushboxes.size(); i2 < max2; i2++) {
			if (is_collide(pushboxes[i].rect, that_pushboxes[i2].rect)) {
				float pushbox_front = pos.x + get_pushbox_front(i);
				float pushbox_back = pos.x + get_pushbox_back(i);
				float that_pushbox_front = that->pos.x + that->get_pushbox_front(i2);
				float that_pushbox_back = that->pos.x + that->get_pushbox_back(i2);

				if (x_diff != 0.0 && that_x_diff != 0.0) {
					//Both players are moving.
					bool forward = x_diff > 0.0 == pos.x < that->pos.x;
					bool that_forward = that_x_diff > 0.0 == that->pos.x < pos.x;
					if (forward == that_forward) {
						if (forward) {
							//Both players are moving forward.
							if (abs(x_diff) == abs(that_x_diff)) {
								//Both players moving forward at the same speed.
								pos.x = prev_pos.x;
								that->pos.x = that->prev_pos.x;
							}
							else if (abs(x_diff) > abs(that_x_diff)) {
								//P1 moving faster than P2.
								pos.x += that_x_diff;
								update_pushbox_pos();
								bool facing_that = x_diff > 0.0 == facing_dir > 0.0;
								bool facing_same = facing_dir == that->facing_dir;
								if (facing_that) {
									if (facing_same) {
										//P1 facing P2, P2 facing away from P1.
										if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
											set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
										}
									}
									else {
										//P1 and P2 facing each other.
										if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
											set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
										}
									}
								}
								else {
									if (facing_same) {
										//P1 is facing away from P2, P2 facing P1.
										if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
											set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
										}
									}
									else {
										//P1 and P2 facing away from each other.
										if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
											set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
										}
									}
								}
							}
							else {
								//P2 is moving faster than P1.
								that->pos.x += x_diff;
								that->update_pushbox_pos();
								bool facing_that = that_x_diff > 0.0 == that->facing_dir > 0.0;
								bool facing_same = facing_dir == that->facing_dir;
								if (facing_that) {
									if (facing_same) {
										if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true)) {
											that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
										}
									}
									else {
										if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true)) {
											that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
										}
									}
								}
								else {
									if (facing_same) {
										if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true)) {
											that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
										}
									}
									else {
										if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true)) {
											that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
										}
									}
								}
							}
						}
						else { 
							//Both players are moving backward.
							bool facing_that = x_diff > 0.0 != facing_dir > 0.0;
							bool facing_same = facing_dir == that->facing_dir;
							if (facing_that) {
								if (facing_same) {
									//P1 facing P2, P2 facing away from P1.
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
								else {
									//P1 and P2 facing each other.
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								if (facing_same) {
									//P1 facing away from P2, P2 facing P1.
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
								else {
									//P1 and P2 facing away from each other.
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
					}
					else if (forward) {
						//P1 moving forward, P2 moving backward.
						bool facing_that = x_diff > 0.0 == facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//P1 moving towards, P2 moving away, P1 front to P2 back.
								if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
								}
							}
							else {
								//P1 moving towards, P2 moving away, face to face.
								if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
								}
							}
						}
						else {
							//P1 moving towards, P2 moving away, P1 back to P2 front.
							if (facing_same) {
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
							else {
								//P1 moving towards, P2 moving away, back to back.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
						}
					}
					else {
						//P2 moving forward, P1 moving backward
						bool facing_that = that_x_diff > 0.0 == that->facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//P2 moving towards, P1 moving away, P2 front to P1 back.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//P2 moving towards, P1 moving away, face to face.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
						else {
							if (facing_same) {
								//P2 moving towards, P1 moving away, P2 back to P1 front.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//P2 moving towards, P1 moving away, back to back.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
					}
				}
				else if (x_diff == 0.0 && that_x_diff == 0.0) {
					bool facing_that = pos.x < that->pos.x == facing_dir > 0.0;
					bool facing_same = facing_dir == that->facing_dir;
					float pos_x = pos.x;
					if (facing_that) {
						if (facing_same) {
							if (abs(that_pushbox_back - pushbox_front) >= 1.0) {
								//No movement, P1 front to P2 back.
								set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
								that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
							}
						}
						else {
							if (abs(that_pushbox_front - pushbox_front) >= 1.0) {
								//No movement, face to face.
								set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
								that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
							}
						}
					}
					else {
						if (facing_same) {
							if (abs(that_pushbox_front - pushbox_back) >= 1.0) {
								//No movement, P1 back to P2 front.
								set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
								that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
							}
						}
						else {
							if (abs(that_pushbox_back - pushbox_back) >= 1.0) {
								//No movement, back to back.
								set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
								that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
							}
						}
					}
				}
				else if (x_diff != 0.0) {
					//P1 is moving, P2 is not.
					if (x_diff > 0.0 == pos.x < that->pos.x) {
						//P1 moving forward.
						bool facing_that = x_diff > 0.0 == facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//Only P1 moving forward, P1 front to P2 back.
								if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
								}
							}
							else {
								//Only P1 moving forward, face to face.
								if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
								}
							}
						}
						else {
							if (facing_same) {
								//Only P1 moving forward, P1 back to P2 front.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
							else {
								//Only P1 moving forward, back to back.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
						}
					}
					else {  
						//P1 moving backward.
						bool facing_that = x_diff > 0.0 != facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//Only P1 moving backward, P1 front to P2 back.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//Only P1 moving backward, face to face.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
						else {
							if (facing_same) {
								//Only P1 moving backward, P1 back to P2 front.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
							else {
								//Only P1 moving backward, back to back.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
						}
					}
				}
				else {
					//P2 is moving, P1 is not.
					if (that_x_diff > 0.0 == that->pos.x < pos.x) {
						//P2 moving forward.
						bool facing_that = that_x_diff > 0.0 == that->facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//Only P2 moving forward, P2 front to P1 back.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//Only P2 moving forward, face to face.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
						else {
							if (facing_same) {
								//Only P2 moving forward, P2 back to P1 front.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//Only P2 moving forward, back to back.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
					}
					else {
						//P2 moving backward.
						bool facing_that = that_x_diff > 0.0 != that->facing_dir > 0.0;
						bool facing_same = facing_dir == that->facing_dir;
						if (facing_that) {
							if (facing_same) {
								//Only P2 moving backward, P2 front to P1 back.
								if (!that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
								}
							}
							else {
								//Only P2 moving backward, face to face.
								if (!that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z), true)) {
									set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
								}
							}
						}
						else {
							if (facing_same) {
								//Only P2 moving backward, P2 back to P1 front.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
							else {
								//Only P2 moving backward, back to back.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true)) {
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z), true);
								}
							}
						}
					}
				}
			}
		}
	}
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = allow_crossup;
	that->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = that_allow_crossup;
	update_pushbox_pos();
	that->update_pushbox_pos();
}

void Fighter::process_projectile_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes) {
	for (size_t i = 0, max = pushboxes.size(); i < max; i++) {
		for (size_t i2 = 0, max2 = that_pushboxes.size(); i2 < max2; i2++) {
			if (is_collide(pushboxes[i].rect, that_pushboxes[i2].rect)) {
				Projectile* that = (Projectile*)that_pushboxes[i2].object;
				//Only they are moving.
				float that_x_diff = that->projectile_float[PROJECTILE_FLOAT_SPEED_X];
				float pushbox_front = pos.x + get_pushbox_front(i);
				float pushbox_back = pos.x + get_pushbox_back(i);
				float that_pushbox_front = that->pos.x + that->get_pushbox_front(i2);
				float that_pushbox_back = that->pos.x + that->get_pushbox_back(i2);

				if (that_x_diff > 0.0 == that->pos.x < pos.x) {
					bool facing_that = that_x_diff > 0.0 == that->facing_dir > 0.0;
					bool facing_same = facing_dir == that->facing_dir;
					if (facing_that) {
						if (facing_same) {
							set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
						}
						else {
							set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
						}
					}
					else {
						if (facing_same) {
							set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
						}
						else {
							set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
						}
					}
				}
				else {  //They're moving away from us
					bool facing_that = that_x_diff > 0.0 != that->facing_dir > 0.0;
					bool facing_same = facing_dir == that->facing_dir;
					if (facing_that) {
						if (facing_same) {
							set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
						}
						else {
							set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
						}
					}
					else {
						if (facing_same) {
							set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
						}
						else {
							set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
						}
					}
				}
			}
		}
	}
}

bool Fighter::is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) {
	if (attacker->fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0
	|| attacker->multihit_connected[hitbox->multihit]) {
		return false;
	}

	if (hurtbox->intangible_kind == hitbox->attack_height
	||	hurtbox->intangible_kind == INTANGIBLE_KIND_STRIKE
	||	(hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT 
		&& !(hitbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))
	||	(hurtbox->intangible_kind == INTANGIBLE_KIND_AERIAL 
		&& attacker->situation_kind == FIGHTER_SITUATION_AIR)) {
		return false;
	}

	switch (situation_kind) {
		case FIGHTER_SITUATION_GROUND: {
			if (!(hitbox->hit_kind & HIT_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_AIR: {
			if ((!(hitbox->hit_kind & HIT_KIND_AIR))
			|| hitbox->juggle_max < fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_DOWN: {
			if (!(hitbox->hit_kind & HIT_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height
			|| fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			if (status_kind == FIGHTER_STATUS_PARRY_START) {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_PARRIED;
			}
			else {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_HITSTUN_PARRIED;
			}
			return true;
		}
	}


	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_RIGHT_OF_WAY) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED;
		return true;
	}

	if (hurtbox->is_armor) {
		if ((hitbox->hit_kind & HIT_KIND_ARMOR) || fighter_flag[FIGHTER_FLAG_ALLOW_ARMOR_BREAK]) {
			fighter_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
			return true;
		}
		else {
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
			return true;
		}
	}

	fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;

	if (situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			switch (hitbox->attack_height) {
				case (ATTACK_HEIGHT_HIGH): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (get_stick_dir() == 6) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() >= 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				default:
				case (ATTACK_HEIGHT_MID): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (!get_stick_dir() % 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
					return true;
				} break;
				case (ATTACK_HEIGHT_LOW): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() < 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
			}
		}
		else if (is_actionable()) {
			switch (hitbox->attack_height) {
				case (ATTACK_HEIGHT_HIGH): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (get_stick_dir() == 6) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				default:
				case (ATTACK_HEIGHT_MID): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (get_stick_dir() == 6 || get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 4 || get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}
					}
				} break;
				case (ATTACK_HEIGHT_LOW): {
					if (attacker->pos.x > pos.x != internal_facing_right) {
						if (get_stick_dir() == 3) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = !internal_facing_right;
							facing_dir = !internal_facing_dir;
							return true;
						}
					}
					else {
						if (get_stick_dir() == 1) {
							incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
							facing_right = internal_facing_right;
							facing_dir = internal_facing_dir;
							return true;
						}

					}
				} break;
			}
		}
	}

	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

bool Fighter::is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) {
	if (attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0
		|| attacker->multihit_connected[hitbox->multihit]) {
		return false;
	}

	if (hurtbox->intangible_kind == hitbox->attack_height
		|| hurtbox->intangible_kind == INTANGIBLE_KIND_PROJECTILE
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT
			&& !(hitbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))) {
		return false;
	}

	switch (situation_kind) {
		case FIGHTER_SITUATION_GROUND: {
			if (!(hitbox->hit_kind & HIT_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_AIR: {
			if ((!(hitbox->hit_kind & HIT_KIND_AIR))
				|| hitbox->juggle_max < fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_DOWN: {
			if (!(hitbox->hit_kind & HIT_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->attack_height
			|| fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_ALL) {
			if (status_kind == FIGHTER_STATUS_PARRY_START) {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_PARRIED;
			}
			else {
				incoming_collision_kind
					= INCOMING_COLLISION_KIND_HITSTUN_PARRIED;
			}
			return true;
		}
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_RIGHT_OF_WAY) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED;
		return true;
	}

	if (hurtbox->is_armor) {
		if ((hitbox->hit_kind & HIT_KIND_ARMOR) || fighter_flag[FIGHTER_FLAG_ALLOW_ARMOR_BREAK]) {
			fighter_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
			return true;
		}
		else {
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
			return true;
		}
	}

	fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;

	if (situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			switch (hitbox->attack_height) {
			case (ATTACK_HEIGHT_HIGH): {
				if (attacker->pos.x > pos.x != internal_facing_right) {
					if (get_stick_dir() == 6) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() >= 4) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			default:
			case (ATTACK_HEIGHT_MID): {
				if (attacker->pos.x > pos.x != internal_facing_right) {
					if (!get_stick_dir() % 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
					facing_right = internal_facing_right;
					facing_dir = internal_facing_dir;
					return true;
				}
				return true;
			} break;
			case (ATTACK_HEIGHT_LOW): {
				if (attacker->pos.x > pos.x != internal_facing_right) {
					if (get_stick_dir() == 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() < 4) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			}
		}
		else if (is_actionable()) {
			switch (hitbox->attack_height) {
			case (ATTACK_HEIGHT_HIGH): {
				if (attacker->pos.x > pos.x != internal_facing_right) {
					if (get_stick_dir() == 6) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() == 4) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			default:
			case (ATTACK_HEIGHT_MID): {
				if (attacker->pos.x > pos.x != internal_facing_right) {
					if (get_stick_dir() == 6 || get_stick_dir() == 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !internal_facing_right;
						facing_dir = !internal_facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() == 4 || get_stick_dir() == 1) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			case (ATTACK_HEIGHT_LOW): {
				if (attacker->pos.x > pos.x != facing_right) {
					if (get_stick_dir() == 3) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = !facing_right;
						facing_dir = !facing_dir;
						return true;
					}
				}
				else {
					if (get_stick_dir() == 1) {
						incoming_collision_kind = INCOMING_COLLISION_KIND_BLOCKED;
						facing_right = internal_facing_right;
						facing_dir = internal_facing_dir;
						return true;
					}
				}
			} break;
			}
		}
	}

	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

void Fighter::process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX] = hitbox->disable_hitstun_parry;
	if (get_status_group() != STATUS_GROUP_HITSTUN) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
	}
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = hitbox->launch_init_y;
	fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = hitbox->launch_gravity_y;
	fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = hitbox->launch_max_fall_speed;
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = hitbox->launch_speed_x * attacker->facing_dir;
	if (fighter_int[FIGHTER_INT_JUGGLE_VALUE] < hitbox->juggle_start) {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
	}
	else {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->juggle_increase;
	}
	float damage;
	float scale;
	int counterhit_val = 0;
	if (attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	if (counterhit_val >= 1) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -counterhit_val;
		scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
		damage = hitbox->damage * scale;
		attacker->fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, attacker->fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		damage += accum_damage;
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
	}
	else {
		scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
		damage = hitbox->damage * scale;
		attacker->fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, attacker->fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		damage += accum_damage;
		fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = false;
	}

	fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
	fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->hit_pushback / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];

	if (!attacker->fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitbox->damage_scale;
	}
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE] = damage;
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE_SCALE] = scale;
	attacker->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
	fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitbox->attack_level;
	
	set_post_collision_status(hitbox, counterhit_val);
	
	if (post_collision_status == FIGHTER_STATUS_HITSTUN_AIR) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * attacker->facing_dir * 0.3;
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 25.0 + (3.0 * hitbox->attack_level);
	}
	fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;

	unique_process_incoming_fighter_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hit(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX] = hitbox->disable_hitstun_parry;
	if (get_status_group() != STATUS_GROUP_HITSTUN) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] = 0;
	}
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = hitbox->launch_init_y;
	fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = hitbox->launch_gravity_y;
	fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = hitbox->launch_max_fall_speed;
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = hitbox->launch_speed_x * attacker->facing_dir;
	if (fighter_int[FIGHTER_INT_JUGGLE_VALUE] < hitbox->juggle_start) {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
	}
	else {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->juggle_increase;
	}
	float damage;
	float scale;
	int counterhit_val = 0;
	if (attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	if (counterhit_val >= 1) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -counterhit_val;
		scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
		damage = hitbox->damage * scale;
		attacker->owner->fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, attacker->owner->fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 1.2, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.72, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		damage += accum_damage;
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
	}
	else {
		scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
		damage = hitbox->damage * scale;
		attacker->owner->fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, attacker->owner->fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.6, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
		float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
		damage += accum_damage;
		fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = false;
	}

	fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
	fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->hit_pushback / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];

	if (!attacker->owner->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS]) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitbox->damage_scale;
	}
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE] = damage;
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE_SCALE] = scale;
	attacker->owner->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
	fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitbox->attack_level;
	set_post_collision_status(hitbox, counterhit_val);
	if (post_collision_status == FIGHTER_STATUS_HITSTUN_AIR) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * attacker->facing_dir * 0.3;
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 25.0 + (3.0 * hitbox->attack_level);
	}
	fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	unique_process_incoming_projectile_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hit(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_COMBO_COUNT]++;
	fighter_flag[FIGHTER_FLAG_ATTACK_HIT] = true;
	unique_process_outgoing_fighter_hitbox_collision_hit(hitbox, defender);
}

void Fighter::process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->multihit);
	unique_process_outgoing_projectile_hitbox_collision_hit(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker) {
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
	fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = round_up_odd(fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
	fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->attack_height;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	if (hitbox->damage_kind == DAMAGE_KIND_CHIP) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	else if (hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->block_pushback / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
	post_collision_status = FIGHTER_STATUS_BLOCKSTUN;
	unique_process_incoming_fighter_hitbox_collision_blocked(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_blocked(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_blocked(hitbox, attacker);
}

void Fighter::process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = true;
	unique_process_outgoing_fighter_hitbox_collision_blocked(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker) {
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker) {
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_projectile_hitbox_collision_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry") * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	unique_process_outgoing_fighter_hitbox_collision_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker) {
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	//TODO: We're actually going to set the hitlag to 1 and have the camera zoom in on the player
	//who parried here.
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 50;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 50;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_hitstun_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker) {
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry"), get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	
	unique_process_incoming_projectile_hitbox_collision_hitstun_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0, fighter_float[FIGHTER_FLOAT_EX_METER] + get_local_param_float("meter_gain_on_parry") * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	fighter_flag[FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY] = true; //Consider using a timer to make
	//it so you can only cancel the recovery 2f after you get hitstun parried. It's fine if getting
	//parried out of your own combo isn't a guaranteed punish, but you should probably still at least
	//be -.
	//TODO: Once the cam animation is implemented, set both of these values to 1.
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 50;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 50;
	unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->damage / 2), fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	unique_process_incoming_fighter_hitbox_collision_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_armored(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_armored(hitbox, attacker);
}

void Fighter::process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(0.0, fighter_float[FIGHTER_FLOAT_EX_METER] + hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	unique_process_outgoing_fighter_hitbox_collision_armored(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker) {
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->damage / 2), fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	unique_process_incoming_fighter_hitbox_collision_right_of_way_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker) {
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - (hitbox->damage / 2), fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	unique_process_incoming_projectile_hitbox_collision_right_of_way_armored(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker) {
	unique_process_incoming_fighter_hitbox_collision_invincibility(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_invincibility(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_invincibility(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_invincibility(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_invincibility(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {
	//Oh yeah I should probably actually write this one
	unique_process_incoming_fighter_hitbox_collision_counter(hitbox, attacker);
}

void Fighter::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {
	unique_process_incoming_projectile_hitbox_collision_counter(hitbox, attacker);
}

void Fighter::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	unique_process_outgoing_fighter_hitbox_collision_counter(hitbox, defender);
}

void Fighter::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->multihit);
	unique_process_outgoing_projectile_hitbox_collision_counter(hitbox, defender);
}

void Fighter::check_incoming_grabbox_collisions(std::vector<Grabbox*> grabboxes) {
	for (size_t i = 0; i < 10; i++) {
		if (!hurtboxes[i].active) continue;
		for (size_t i2 = 0, max2 = grabboxes.size(); i2 < max2; i2++) {
			if (is_valid_incoming_grabbox_collision(&hurtboxes[i], grabboxes[i2])) {
				connected_grabbox = grabboxes[i2];
				return;
			}
		}
	}
}

bool Fighter::is_valid_incoming_grabbox_collision(Hurtbox* hurtbox, Grabbox* grabbox) {
	if (!is_collide(hurtbox->rect, grabbox->rect)) return false;
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_INVINCIBLE) {
		grabbox->clear();
		return false;
	}
	if (hurtbox->intangible_kind == INTANGIBLE_KIND_THROW
		|| (hurtbox->intangible_kind == INTANGIBLE_KIND_SOFT
			&& !(grabbox->hit_kind & HIT_KIND_SOFT_INTANGIBLE))) {
		return false;
	}
	if (get_status_group() == STATUS_GROUP_HITSTUN && !(grabbox->grabbox_kind & GRABBOX_KIND_HITSTUN)) {
		return false;
	}
	switch (situation_kind) {
	case FIGHTER_SITUATION_GROUND: {
		return grabbox->hit_kind & HIT_KIND_GROUND;
	} break;
	case FIGHTER_SITUATION_AIR: {
		return grabbox->hit_kind & HIT_KIND_AIR;
	} break;
	case FIGHTER_SITUATION_DOWN: {
		return grabbox->hit_kind & HIT_KIND_DOWN;
	} break;
	default: {
		return false;
	} break;
	}
}

void Fighter::process_incoming_grabbox_collision(Grabbox* grabbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_grabbox_collision(grabbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_grabbox_collision(grabbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void Fighter::process_incoming_fighter_grabbox_collision(Grabbox* grabbox, Fighter* attacker) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && fighter_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		post_collision_status = grabbox->defender_status_if_hit;
	}
	unique_process_incoming_fighter_grabbox_collision(grabbox, attacker);
	attacker->process_outgoing_fighter_grabbox_collision(grabbox, this);
}

void Fighter::process_incoming_projectile_grabbox_collision(Grabbox* grabbox, Projectile* attacker) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && fighter_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		post_collision_status = grabbox->defender_status_if_hit;
		fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	}
	unique_process_incoming_projectile_grabbox_collision(grabbox, attacker);
	attacker->process_outgoing_fighter_grabbox_collision(grabbox, this);
}

void Fighter::process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && defender->fighter_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		post_collision_status = grabbox->attacker_status_if_hit;
	}
	unique_process_outgoing_fighter_grabbox_collision(grabbox, defender);
}

int Fighter::get_counterhit_val(Hitbox* hitbox) {
	//Note: Returning 1 means we apply the damage multipliers, returning 2 means we apply the
	//specific counterhit status as well.
	Fighter* attacker = nullptr;
	if (hitbox->object->object_type == BATTLE_OBJECT_TYPE_FIGHTER) {
		attacker = (Fighter*)hitbox->object;
	}
	else {
		attacker = (Fighter*)((Projectile*)hitbox->object)->owner;
	}
	if (anim_kind != nullptr) {
		if (status_kind == FIGHTER_STATUS_HITSTUN_PARRY_START
			|| status_kind == FIGHTER_STATUS_LAUNCH_PARRY_START) {
			attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_HITSTUN_PARRY;
			return 2;
		}
		else {
			switch (hitbox->counterhit_type) {
				case (COUNTERHIT_TYPE_COUNTER):
				default: {
					if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
						return 2;
					}
				} break;
				case (COUNTERHIT_TYPE_ANY): {
					if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] || fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
						if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
							attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
						}
						if (fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
							attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_PUNISH;
						}
						return 2;
					}
				} break;
				case (COUNTERHIT_TYPE_PUNISH): {
					if (fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_PUNISH;
						return 2;
					}
					else if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
						return 1;
					}
				} break;
				case (COUNTERHIT_TYPE_JUMP_COUNTER): {
					if (fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_JUMP;
						return 2;
					}
					else if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
						return 1;
					}
				} break;
				case (COUNTERHIT_TYPE_NONE): {
					if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
						attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
						return 1;
					}
				} break;
			}
		}
	}
	return 0;
}

void Fighter::set_post_collision_status(Hitbox* hitbox, int counterhit_val) {
	int hit_status = hitbox->hit_status;
	if (counterhit_val == 2) {
		hit_status = hitbox->counterhit_status;
	}

	if (fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0f &&
		GameManager::get_instance()->get_game_state()->game_context != GAME_CONTEXT_TRAINING) {
		if (hitbox->attack_level == ATTACK_LEVEL_LIGHT && situation_kind == FIGHTER_SITUATION_GROUND) {
			post_collision_status = FIGHTER_STATUS_CRUMPLE;
		}
		else {
			switch (situation_kind) {
			case (FIGHTER_SITUATION_GROUND): {
				if (hit_status == HIT_STATUS_LAUNCH) {
					post_collision_status = FIGHTER_STATUS_LAUNCH_START;
				}
				else {
					post_collision_status = FIGHTER_STATUS_KNOCKDOWN_START;
				}
			} break;
			case (FIGHTER_SITUATION_AIR): {
				post_collision_status = FIGHTER_STATUS_LAUNCH;
			} break;
			case (FIGHTER_SITUATION_DOWN): {
				post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
			} break;
			}

		}
	}
	if (hitbox->continue_launch && (status_kind == FIGHTER_STATUS_LAUNCH
		|| status_kind == FIGHTER_STATUS_HITSTUN_FLOAT)) {
		post_collision_status = FIGHTER_STATUS_LAUNCH;
		return;
	}
	switch (hit_status) {
	case (HIT_STATUS_NORMAL):
	default: {
		switch (situation_kind) {
		case (FIGHTER_SITUATION_AIR): {
			post_collision_status = FIGHTER_STATUS_HITSTUN_AIR;
		} break;
		case (FIGHTER_SITUATION_GROUND): {
			post_collision_status = FIGHTER_STATUS_HITSTUN;
		} break;
		case (FIGHTER_SITUATION_DOWN): {
			post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
		} break;
		}
	} break;
	case (HIT_STATUS_CRUMPLE): {
		switch (situation_kind) {
		case (FIGHTER_SITUATION_AIR): {
			post_collision_status = FIGHTER_STATUS_LAUNCH;
		} break;
		case (FIGHTER_SITUATION_GROUND): {
			post_collision_status = FIGHTER_STATUS_CRUMPLE;
		} break;
		case (FIGHTER_SITUATION_DOWN): {
			post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
		} break;
		}
	} break;
	case (HIT_STATUS_FLOAT): {
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			post_collision_status = FIGHTER_STATUS_HITSTUN_AIR;
		}
		else {
			post_collision_status = FIGHTER_STATUS_HITSTUN_FLOAT;
		}
	} break;
	case (HIT_STATUS_LAUNCH): {
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			post_collision_status = FIGHTER_STATUS_LAUNCH;
		}
		else {
			post_collision_status = FIGHTER_STATUS_LAUNCH_START;
		}
	} break;
	case (HIT_STATUS_KNOCKDOWN): {
		if (situation_kind == FIGHTER_SITUATION_DOWN) {
			post_collision_status = FIGHTER_STATUS_KNOCKDOWN;
		}
		else {
			post_collision_status = FIGHTER_STATUS_KNOCKDOWN_START;
		}
	} break;
	}
}