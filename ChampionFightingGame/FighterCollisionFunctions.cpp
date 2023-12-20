#include "Fighter.h"
#include "Projectile.h"
#include "utils.h"

void Fighter::process_fighter_pushbox_collisions(std::vector<Pushbox> pushboxes, std::vector<Pushbox> that_pushboxes) {
	//This function only runs for P1
	Fighter* that = object_manager->fighter[1];
	float x_diff = fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED];
	float that_x_diff = that->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED];
	bool allow_crossup = fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP];
	bool that_allow_crossup = that->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP];
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	that->fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	if (pos.x == that->pos.x && prev_pos.x != that->prev_pos.x) {
		if (pos.x == stage->stage_bounds.x) {
			if (prev_pos.x != pos.x) {
				pos.x++;
			}
			else if (that->prev_pos.x != that->pos.x) {
				that->pos.x++;
			}
		}
		if (pos.x == stage->stage_bounds.y) {
			if (prev_pos.x != pos.x) {
				pos.x--;
			}
			else if (that->prev_pos.x != that->pos.x) {
				that->pos.x--;
			}
		}
		update_pushbox_pos();
		that->update_pushbox_pos();
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
								bool facing_that = pos.x < that->pos.x == facing_dir > 0.0;
								bool facing_same = facing_dir == that->facing_dir;
								float pos_x = pos.x;
								if (facing_that) {
									if (facing_same) {
										if (abs(that_pushbox_back - pushbox_front) >= 1.0) {
											//Both forward, P1 front to P2 back.
											set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z), true);
											that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
											add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
										}
									}
									else {
										if (abs(that_pushbox_front - pushbox_front) >= 1.0) {
											//Both forward, face to face.
											set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z), true);
											that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
											add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
										}
									}
								}
								else {
									if (facing_same) {
										if (abs(that_pushbox_front - pushbox_back) >= 1.0) {
											//Both forward, P1 back to P2 front.
											set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z), true);
											that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
											add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
										}
									}
									else {
										if (abs(that_pushbox_back - pushbox_back) >= 1.0) {
											//Both forward, back to back.
											set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z), true);
											that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
											add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
										}
									}
								}
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
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_front(i), pos.y, pos.z))) {
									pushbox_front = pos.x + get_pushbox_front(i);
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_back(i2), that->pos.y, that->pos.z));
								}
								else {
									that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
									add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								}
							}
						}
						else {
							if (abs(that_pushbox_front - pushbox_front) >= 1.0) {
								//No movement, face to face.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_front(i), pos.y, pos.z))) {
									pushbox_front = pos.x + get_pushbox_front(i);
									that->set_pos(glm::vec3(pushbox_front - that->get_pushbox_front(i2), that->pos.y, that->pos.z));
								}
								else {
									that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
									add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								}
							}
						}
					}
					else {
						if (facing_same) {
							if (abs(that_pushbox_front - pushbox_back) >= 1.0) {
								//No movement, P1 back to P2 front.
								if (!set_pos(glm::vec3(that_pushbox_front - get_pushbox_back(i), pos.y, pos.z))) {
									pushbox_back = pos.x + get_pushbox_back(i);
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_front(i2), that->pos.y, that->pos.z));
								}
								else {
									that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
									add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								}
							}
						}
						else {
							if (abs(that_pushbox_back - pushbox_back) >= 1.0) {
								//No movement, back to back.
								if (!set_pos(glm::vec3(that_pushbox_back - get_pushbox_back(i), pos.y, pos.z))) {
									pushbox_back = pos.x + get_pushbox_back(i);
									that->set_pos(glm::vec3(pushbox_back - that->get_pushbox_back(i2), that->pos.y, that->pos.z));
								}
								else {
									that->add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
									add_pos(glm::vec3(abs(pos_x - pos.x) / 2 * facing_dir, 0, 0));
								}
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

	switch (hitbox->hit_height) {
		case (HIT_HEIGHT_HIGH): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_HIGH) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_MID): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_MID) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_LOW): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_LOW) {
				return false;
			}
		} break;
		default: {
			return false;
		}
	}

	if (((hurtbox->intangible_kind & INTANGIBLE_KIND_SOFT) && !(hitbox->collision_kind & COLLISION_KIND_SOFT_INTANGIBLE))
	||	((hurtbox->intangible_kind & INTANGIBLE_KIND_AERIAL) && attacker->situation_kind == FIGHTER_SITUATION_AIR)) {
		return false;
	}

	switch (situation_kind) {
		case FIGHTER_SITUATION_GROUND: {
			if (!(hitbox->collision_kind & COLLISION_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_AIR: {
			if ((!(hitbox->collision_kind & COLLISION_KIND_AIR))
			|| (hitbox->juggle_max != -1 &&
				hitbox->juggle_max < fighter_int[FIGHTER_INT_JUGGLE_VALUE])) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_DOWN: {
			if (!(hitbox->collision_kind & COLLISION_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->hit_height
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

	if (hurtbox->armor_hits) {
		if ((hitbox->collision_kind & COLLISION_KIND_ARMOR) || fighter_int[FIGHTER_INT_ARMOR_HITS] == hurtbox->armor_hits) {
			fighter_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
		}
		else {
			fighter_int[FIGHTER_INT_ARMOR_HITS]++;
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
		}
		return true;
	}

	fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = false;

	if (situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
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
				case (HIT_HEIGHT_MID): {
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
				case (HIT_HEIGHT_LOW): {
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
			switch (hitbox->hit_height) {
				case (HIT_HEIGHT_HIGH): {
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
				case (HIT_HEIGHT_MID): {
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
				case (HIT_HEIGHT_LOW): {
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

	switch (hitbox->hit_height) {
		case (HIT_HEIGHT_HIGH): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_HIGH) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_MID): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_MID) {
				return false;
			}
		} break;
		case (HIT_HEIGHT_LOW): {
			if (hurtbox->intangible_kind & INTANGIBLE_KIND_LOW) {
				return false;
			}
		} break;
		default: {
			return false;
		}
	}

	if ((hurtbox->intangible_kind & INTANGIBLE_KIND_PROJECTILE)
		|| ((hurtbox->intangible_kind & INTANGIBLE_KIND_SOFT) && !(hitbox->collision_kind & COLLISION_KIND_SOFT_INTANGIBLE))) {
		return false;
	}

	switch (situation_kind) {
		case FIGHTER_SITUATION_GROUND: {
			if (!(hitbox->collision_kind & COLLISION_KIND_GROUND)) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_AIR: {
			if ((!(hitbox->collision_kind & COLLISION_KIND_AIR))
				|| (hitbox->juggle_max != -1 &&
					hitbox->juggle_max < fighter_int[FIGHTER_INT_JUGGLE_VALUE])) {
				return false;
			}
		} break;
		case FIGHTER_SITUATION_DOWN: {
			if (!(hitbox->collision_kind & COLLISION_KIND_DOWN)) {
				return false;
			}
		} break;
		default: {
			return false;
		} break;
	}

	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_INVINCIBILITY;
		return true;
	}

	if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE]) {
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == hitbox->hit_height
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

	if (hurtbox->armor_hits) {
		if ((hitbox->collision_kind & COLLISION_KIND_ARMOR) || fighter_int[FIGHTER_INT_ARMOR_HITS] == hurtbox->armor_hits) {
			fighter_flag[FIGHTER_FLAG_ARMOR_BREAK] = true;
			incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
		}
		else {
			fighter_int[FIGHTER_INT_ARMOR_HITS]++;
			incoming_collision_kind = INCOMING_COLLISION_KIND_ARMORED;
		}
		return true;
	}

	fighter_flag[FIGHTER_FLAG_LAST_HIT_WAS_PROJECTILE] = true;

	if (situation_kind == FIGHTER_SITUATION_GROUND && hitbox->chip_damage != -1.0) {
		if (status_kind == FIGHTER_STATUS_BLOCKSTUN) {
			switch (hitbox->hit_height) {
			case (HIT_HEIGHT_HIGH): {
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
			case (HIT_HEIGHT_MID): {
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
			case (HIT_HEIGHT_LOW): {
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
			switch (hitbox->hit_height) {
			case (HIT_HEIGHT_HIGH): {
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
			case (HIT_HEIGHT_MID): {
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
			case (HIT_HEIGHT_LOW): {
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
	fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = hitbox->hit_flags & HIT_FLAG_DISABLE_HITSTUN_PARRY;
	if (!fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
	}
	else {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->juggle_increase;
	}
	float damage;
	float scale;
	int counterhit_val = COUNTERHIT_VAL_NONE;
	if (attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	switch (counterhit_val) {
		case (COUNTERHIT_VAL_SPECIAL_STATUS): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -2;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
		} break;
		case (COUNTERHIT_VAL_PUNISH): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -2;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		} break;
		case (COUNTERHIT_VAL_COUNTER): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -1;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		} break;
		default:
		case (COUNTERHIT_VAL_NONE): {
			if (attacker->fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
				scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_PREV_DAMAGE_SCALE], 12)) / 10;
			}
			else {
				scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			}
			damage = hitbox->damage * scale;
			attacker->gain_ex(hitbox->meter_gain);
			gain_ex(hitbox->meter_gain * 0.6);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
		} break;
	}

	if (!attacker->fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE_FOR_UI] = fighter_int[FIGHTER_INT_DAMAGE_SCALE];
		fighter_int[FIGHTER_INT_PREV_DAMAGE_SCALE] = fighter_int[FIGHTER_INT_DAMAGE_SCALE];
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitbox->damage_scale;
	}
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE] = damage;
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE_SCALE] = scale;
	attacker->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
	fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitbox->hit_level;
	set_post_collision_status(hitbox, counterhit_val);

	fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;

	unique_process_incoming_fighter_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hit(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->hitstun;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 0;
	fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = hitbox->hit_flags & HIT_FLAG_DISABLE_HITSTUN_PARRY;
	if (!fighter_int[FIGHTER_INT_JUGGLE_VALUE]) {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] = hitbox->juggle_start;
	}
	else {
		fighter_int[FIGHTER_INT_JUGGLE_VALUE] += hitbox->juggle_increase;
	}
	float damage;
	float scale;
	int counterhit_val = COUNTERHIT_VAL_NONE;
	if (attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL]) {
		counterhit_val = attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL];
	}
	else {
		counterhit_val = get_counterhit_val(hitbox);
		attacker->owner->fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = counterhit_val;
	}

	switch (counterhit_val) {
		case (COUNTERHIT_VAL_SPECIAL_STATUS): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -2;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->owner->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
		} break;
		case (COUNTERHIT_VAL_PUNISH): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -2;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->owner->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		} break;
		case (COUNTERHIT_VAL_COUNTER): {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -1;
			scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			damage = hitbox->damage * scale;
			attacker->owner->gain_ex(hitbox->meter_gain * 1.2);
			gain_ex(hitbox->meter_gain * 0.72);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
			fighter_int[FIGHTER_INT_HITSTUN_FRAMES] += 3;
		} break;
		default:
		case (COUNTERHIT_VAL_NONE): {
			if (attacker->projectile_flag[PROJECTILE_FLAG_ATTACK_HIT]) {
				scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_PREV_DAMAGE_SCALE], 12)) / 10;
			}
			else {
				scale = (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 12)) / 10;
			}
			damage = hitbox->damage * scale;
			attacker->owner->gain_ex(hitbox->meter_gain);
			gain_ex(hitbox->meter_gain * 0.6);
			float accum_damage = fighter_float[FIGHTER_FLOAT_HEALTH] - fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH];
			fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(hitbox->damage_kind == DAMAGE_KIND_NO_KO, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
			damage += accum_damage;
		} break;
	}

	if (!attacker->projectile_flag[PROJECTILE_FLAG_ATTACK_HIT]) {
		fighter_int[FIGHTER_INT_DAMAGE_SCALE_FOR_UI] = fighter_int[FIGHTER_INT_DAMAGE_SCALE];
		fighter_int[FIGHTER_INT_PREV_DAMAGE_SCALE] = fighter_int[FIGHTER_INT_DAMAGE_SCALE];
		fighter_int[FIGHTER_INT_DAMAGE_SCALE] += hitbox->damage_scale;
	}

	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE] = damage;
	fighter_float[FIGHTER_FLOAT_LAST_DAMAGE_SCALE] = scale;
	attacker->owner->fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
	fighter_int[FIGHTER_INT_HITSTUN_LEVEL] = hitbox->hit_level;
	set_post_collision_status(hitbox, counterhit_val);

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
	gain_ex(hitbox->meter_gain * 0.3);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
	fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->pushback_frames;
	fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->hit_height;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	if (hitbox->damage_kind == DAMAGE_KIND_CHIP) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	else if (hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->pushback_ground_block / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
	post_collision_status = FIGHTER_STATUS_BLOCKSTUN;
	fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	unique_process_incoming_fighter_hitbox_collision_blocked(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_blocked(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(hitbox->meter_gain * 0.3);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = hitbox->blockstun;
	fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->pushback_frames;
	fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] = hitbox->hit_height;
	fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	if (hitbox->damage_kind == DAMAGE_KIND_CHIP) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(1.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	else if (hitbox->damage_kind == DAMAGE_KIND_CHIP_KO) {
		fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, fighter_float[FIGHTER_FLOAT_HEALTH] - hitbox->chip_damage, fighter_float[FIGHTER_FLOAT_HEALTH]);
	}
	fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->pushback_ground_block / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
	post_collision_status = FIGHTER_STATUS_BLOCKSTUN;
	fighter_int[FIGHTER_INT_TRAINING_HEALTH_RECOVERY_TIMER] = 60;
	unique_process_incoming_projectile_hitbox_collision_blocked(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_blocked(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(hitbox->meter_gain * 0.5);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->blocklag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = true;
	unique_process_outgoing_fighter_hitbox_collision_blocked(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker) {
	gain_ex(get_local_param_float("meter_gain_on_parry"));
	if (fighter_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY]) {
		start_cinematic_sequence("hitstun_parry", 1.0, 1.0, 0.6, false, 0.02);
		fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 58;
		fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 58;
	}
	else {
		fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	}
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(get_local_param_float("meter_gain_on_parry"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_projectile_hitbox_collision_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(get_local_param_float("meter_gain_on_parry") * 0.5);
	if (defender->fighter_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY]) {
		fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 1;
		fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 1;
	}
	else {
		fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
		fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	}
	unique_process_outgoing_fighter_hitbox_collision_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker) {
	gain_ex(get_local_param_float("meter_gain_on_parry"));
	start_cinematic_sequence("hitstun_parry", 1.0, 1.0, 0.6, false, 0.02);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 58;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 58;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	unique_process_incoming_fighter_hitbox_collision_hitstun_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker) {
	gain_ex(get_local_param_float("meter_gain_on_parry"));
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 16;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 16;
	fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = fighter_float[FIGHTER_FLOAT_HEALTH];
	post_collision_status = FIGHTER_STATUS_PARRY;
	
	unique_process_incoming_projectile_hitbox_collision_hitstun_parried(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	gain_ex(get_local_param_float("meter_gain_on_parry") * 0.5);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 1;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 1;
	fighter_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED] = true;
	enable_cancel(CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
	unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag / 2;
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
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag / 2;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	gain_ex(hitbox->meter_gain * 0.3);
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
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag / 2;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = fighter_int[FIGHTER_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_invincibility(hitbox, defender);
}

void Fighter::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	post_collision_status = fighter_int[FIGHTER_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_fighter_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_counter(hitbox, this);
}

void Fighter::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	post_collision_status = fighter_int[FIGHTER_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_projectile_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_fighter_hitbox_collision_counter(hitbox, this);
}

void Fighter::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	post_collision_status = defender->fighter_int[FIGHTER_INT_COUNTER_ATTACKER_STATUS];
	unique_process_outgoing_fighter_hitbox_collision_counter(hitbox, defender);
}

void Fighter::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->multihit);
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = hitbox->hitlag;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	post_collision_status = defender->projectile_int[PROJECTILE_INT_COUNTER_ATTACKER_STATUS];
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
	if (hurtbox->intangible_kind & INTANGIBLE_KIND_INVINCIBLE) {
		grabbox->clear();
		return false;
	}
	if ((hurtbox->intangible_kind & INTANGIBLE_KIND_THROW)
	|| ((hurtbox->intangible_kind & INTANGIBLE_KIND_SOFT) && !(grabbox->collision_kind & COLLISION_KIND_SOFT_INTANGIBLE))) {
		return false;
	}
	if (get_status_group() == STATUS_GROUP_HITSTUN && !(grabbox->grabbox_kind & GRABBOX_KIND_HITSTUN)) {
		return false;
	}
	switch (situation_kind) {
		case FIGHTER_SITUATION_GROUND: {
			return grabbox->collision_kind & COLLISION_KIND_GROUND;
		} break;
		case FIGHTER_SITUATION_AIR: {
			return grabbox->collision_kind & COLLISION_KIND_AIR;
		} break;
		case FIGHTER_SITUATION_DOWN: {
			return grabbox->collision_kind & COLLISION_KIND_DOWN;
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
		fighter_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = (grabbox->grabbox_kind & GRABBOX_KIND_NOTECH);
		post_collision_status = grabbox->defender_status_if_hit;
		fighter_int[FIGHTER_INT_PARTIAL_HEALTH_FRAMES] = 60;
	}
	unique_process_incoming_fighter_grabbox_collision(grabbox, attacker);
	attacker->process_outgoing_fighter_grabbox_collision(grabbox, this);
}

void Fighter::process_incoming_projectile_grabbox_collision(Grabbox* grabbox, Projectile* attacker) {
	if ((!(grabbox->grabbox_kind & GRABBOX_KIND_NOTECH)) && fighter_flag[FIGHTER_FLAG_THROW_TECH]) {
		post_collision_status = FIGHTER_STATUS_THROW_TECH;
	}
	else {
		fighter_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = (grabbox->grabbox_kind & GRABBOX_KIND_NOTECH);
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
	if (status_kind == FIGHTER_STATUS_HITSTUN_PARRY_START
		|| status_kind == FIGHTER_STATUS_LAUNCH_PARRY_START) {
		attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_HITSTUN_PARRY;
		if (hitbox->special_status_condition != SPECIAL_STATUS_CONDITION_NONE) {
			return COUNTERHIT_VAL_SPECIAL_STATUS;
		}
		else {
			return COUNTERHIT_VAL_PUNISH;
		}
	}
	if (fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] 
		&& hitbox->special_status_condition & SPECIAL_STATUS_CONDITION_JUMP_COUNTERHIT) {
		attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_JUMP;
		return COUNTERHIT_VAL_SPECIAL_STATUS;
	}
	if (fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH]) {
		attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER_PUNISH;
		if (hitbox->special_status_condition & SPECIAL_STATUS_CONDITION_PUNISH) {
			return COUNTERHIT_VAL_SPECIAL_STATUS;
		}
		return COUNTERHIT_VAL_PUNISH;
	}
	if (fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT]) {
		attacker->fighter_int[FIGHTER_INT_UI_TEXT_TYPE] = UI_TEXT_TYPE_COUNTER;
		if (hitbox->special_status_condition & SPECIAL_STATUS_CONDITION_COUNTERHIT) {
			return COUNTERHIT_VAL_SPECIAL_STATUS;
		}
		return COUNTERHIT_VAL_COUNTER;
	}

	return COUNTERHIT_VAL_NONE;
}

void Fighter::set_post_collision_status(Hitbox* hitbox, int counterhit_val) {
	BattleObject* attacker = nullptr;
	if (hitbox->object->object_type == BATTLE_OBJECT_TYPE_FIGHTER) {
		attacker = (Fighter*)hitbox->object;
	}
	else {
		attacker = (Projectile*)hitbox->object;
	}
	int hit_status = hitbox->hit_status;
	HitFlag hit_flags = hitbox->hit_flags;
	if (counterhit_val == COUNTERHIT_VAL_SPECIAL_STATUS) {
		hit_status = hitbox->special_status;
		hit_flags = hitbox->special_hit_flags;
	}

	if (fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0f &&
		GameManager::get_instance()->get_game_state()->game_context != GAME_CONTEXT_TRAINING) {
		if (hitbox->hit_level == HIT_LEVEL_LIGHT && situation_kind == FIGHTER_SITUATION_GROUND) {
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
	else if (hit_flags & HIT_FLAG_CONTINUE_LAUNCH && (status_kind == FIGHTER_STATUS_LAUNCH
		|| status_kind == FIGHTER_STATUS_HITSTUN_FLOAT)) {
		post_collision_status = FIGHTER_STATUS_LAUNCH;
	}
	else {
		switch (hit_status) {
			case (HIT_STATUS_NORMAL): {
				if (hit_flags & HIT_FLAG_FORCE_AERIAL) {
					situation_kind = FIGHTER_SITUATION_AIR;
				}
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
					post_collision_status = FIGHTER_STATUS_LAUNCH;
					fighter_flag[FIGHTER_FLAG_LAUNCH_FLOAT] = true;
					fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = true;
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
			case (HIT_STATUS_CUSTOM): {
				if (counterhit_val == COUNTERHIT_VAL_SPECIAL_STATUS) {
					post_collision_status = hitbox->custom_special_status;
				}
				else {
					post_collision_status = hitbox->custom_hit_status;
				}
			} break;
			default:
			case (HIT_STATUS_NONE): {

			} break;
		}
	}
	switch (post_collision_status) {
		case (FIGHTER_STATUS_HITSTUN): {
			if (hit_flags & HIT_FLAG_FORCE_STAND) {
				fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_FORCE_STAND;
			}
			if (hit_flags & HIT_FLAG_FORCE_CROUCH) {
				fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_FORCE_CROUCH;
			}
			fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->pushback_frames;
			fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->pushback_ground_hit / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
		} break;
		case (FIGHTER_STATUS_HITSTUN_AIR): {
			fighter_int[FIGHTER_INT_PUSHBACK_FRAMES] = hitbox->pushback_frames;
			fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] = hitbox->pushback_air_x / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_PUSHBACK_PER_FRAME] * attacker->facing_dir;
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = hitbox->pushback_air_y / fighter_int[FIGHTER_INT_PUSHBACK_FRAMES];
		} break;
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH): {
			fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = hitbox->launch_init_y;
			fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = hitbox->launch_gravity_y;
			fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = hitbox->launch_max_fall_speed;
			fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] = hitbox->launch_speed_x * attacker->facing_dir;
		}
		case (FIGHTER_STATUS_CRUMPLE):
		case (FIGHTER_STATUS_KNOCKDOWN):
		case (FIGHTER_STATUS_HITSTUN_FLOAT): {
			fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = hit_flags & HIT_FLAG_HARD_KNOCKDOWN;
			fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = hit_flags & HIT_FLAG_KNOCKDOWN_FACE_DOWN;
		} break;
		default: {

		} break;
	}
}