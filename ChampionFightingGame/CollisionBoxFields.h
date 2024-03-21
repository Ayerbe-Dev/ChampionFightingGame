#pragma once

enum CollisionKind : unsigned char {
	COLLISION_KIND_GROUND = 1,
	COLLISION_KIND_AIR = 2,
	COLLISION_KIND_DOWN = 4,
	COLLISION_KIND_PROJECTILE = 8,
	COLLISION_KIND_ARMOR = 16,

	COLLISION_KIND_MAX = 255,
};

inline CollisionKind operator|(CollisionKind a, CollisionKind b) {
	return static_cast<CollisionKind>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum HitHeight {
	HIT_HEIGHT_HIGH,
	HIT_HEIGHT_MID,
	HIT_HEIGHT_LOW,

	HIT_HEIGHT_MAX,
};

enum HitStatus {
	HIT_STATUS_NORMAL,
	HIT_STATUS_CRUMPLE,
	HIT_STATUS_FLOAT,
	HIT_STATUS_LAUNCH,
	HIT_STATUS_KNOCKDOWN,
	HIT_STATUS_CUSTOM,
	HIT_STATUS_NONE,

	HIT_STATUS_MAX,
};

struct HitResult {
	HitResult() {
		hitlag = 0;
		hitstun = 0;
		crit_hitlag = -1;
		crit_hitstun = -1;
		blocklag = 0;
		blockstun = 0;

		base_damage = 0.0f;
		chip_percent = 0.2f;
		meter_gain = 0.0f;

		damage_scale_add = 0.1f;
		damage_scale_combo_start = 0.9f;

		juggle_start = 0;
		juggle_min = 0;
		juggle_increase = 0;
		juggle_max = 0;

		stand_hit_anim = "stand_hitstun_light_mid";
		crouch_hit_anim = "crouch_hitstun_light_mid";
		stand_block_anim = "stand_blockstun_mid";
		crouch_block_anim = "crouch_blockstun_mid";
	}

	/*
	* Sets the base damage of a HitResult before returning a copy of it. 
	* 
	* If this function is not called, the default value for base_damage is 0.0f.
	*/
	HitResult damage(float base_damage) {
		HitResult ret = *this;
		ret.base_damage = base_damage;
		return ret;
	}

	/*
	* Sets the chip damage percent of a HitResult before returning a copy of it.
	* 
	* If this function is not called, the default value for chip_percent is 0.2f.
	*/
	HitResult chip(float chip_percent) {
		HitResult ret = *this;
		ret.chip_percent = chip_percent;
		return ret;
	}

	/*
	* Sets the meter gain of a HitResult before returning a copy of it. On a regular hit, this value
	* is added to the attacker's meter, and a fraction of it is added to the defender's meter. On other
	* types of hits (counter, block, armor, etc.), both the attacker and defender's meter gain are 
	* further multiplied depending on what type of hit it was.
	* 
	* If this function is not called, the default value for meter_gain is 0.0f.
	*/
	HitResult meter(float meter_gain) {
		HitResult ret = *this;
		ret.meter_gain = meter_gain;
		return ret;
	}

	/*
	* Sets the damage scale of a HitResult before returning a copy of it. This value is added to the
	* defender's damage scaling the first time the attacker hits them during their animation (I.E. 
	* if a single move hits multiple times, this value is only added when the first hit connects). 
	*
	* If this function is not called, the default value for damage_scale_add is 0.1f.
	*/
	HitResult scale(float damage_scale_add) {
		HitResult ret = *this;
		ret.damage_scale_add = damage_scale_add;
		return ret;
	}

	/*
	* Sets the combo start damage scale of a HitResult before returning a copy of it. For the first hit
	* of a combo, the opponent's damage scale is set to this value.
	*
	* If this function is not called, the default value for damage_scale_combo_start is 0.9f.
	*/
	HitResult start_scale(float damage_scale_combo_start) {
		HitResult ret = *this;
		ret.damage_scale_combo_start = damage_scale_combo_start;
		return ret;
	}

	/*
	* Sets the starting juggle value of a HitResult before returning a copy of it. When the opponent
	* is put into either air hitstun or a launch state out of any other state, their juggle value is
	* set to this value. 
	* 
	* If this function is not called, the default value for juggle_start is 0.
	*/
	HitResult j_start(int juggle_start) {
		HitResult ret = *this;
		ret.juggle_start = juggle_start;
		return ret;
	}

	/*
	* Sets the minimum juggle value of a HitResult before returning a copy of it. If the opponent is
	* in a juggle state and their juggle value is less than this value, their juggle value is 
	* immediately set to this one.
	*
	* If this function is not called, the default value for juggle_min is 0.
	*/
	HitResult j_min(int juggle_min) {
		HitResult ret = *this;
		ret.juggle_min = juggle_min;
		return ret;
	}

	/*
	* Sets the juggle increase value of a HitResult before returning a copy of it. If the opponent is
	* in a juggle state and their juggle value is greater than or equal to juggle_min, their juggle 
	* value is increased by this value.
	*
	* If this function is not called, the default value for juggle_increase is 0.
	*/
	HitResult j_inc(int juggle_increase) {
		HitResult ret = *this;
		ret.juggle_increase = juggle_increase;
		return ret;
	}

	/*
	* Sets the max juggle value of a HitResult before returning a copy of it. If the opponent has a
	* juggle value greater than this value, the hitbox that this is attached to may not connect. If
	* this value is set to -1, it may connect regardless of the opponent's juggle value.
	*
	* If this function is not called, the default value for juggle_increase is 0.
	*/
	HitResult j_max(int juggle_max) {
		HitResult ret = *this;
		ret.juggle_max = juggle_max;
		return ret;
	}

	/*
	* Sets the hitlag and hitstun values of a HitResult before returning a copy of it. The hitstun
	* value behaves differently depending on the status this hitbox puts the opponent into. For regular
	* hitstun, it determines how long the opponent is vulnerable. For launch and knockdown, it determines
	* how long they're in the bounce animation after landing. For crumple and float, it determines how
	* long they're in the initial crumple and float animations, respectively.
	* 
	* If this function is not called, the default values for hitlag and hitstun are 0.
	*/
	HitResult hit(int hitlag, int hitstun) {
		HitResult ret = *this;
		ret.hitlag = hitlag;
		ret.hitstun = hitstun;
		return ret;
	}

	/*
	* Sets the critical hitlag and hitstun values of a HitResult before returning a copy of it. On
	* critical hit, if these values are set, they will be used instead of the regular hitstun values.
	* 
	* If this function is not called, the default values for critical hitlag and hitstun are -1,
	* indicating that they are unset.
	*/
	HitResult crit(int hitlag, int hitstun) {
		HitResult ret = *this;
		ret.crit_hitlag = hitlag;
		ret.crit_hitstun = hitstun;
		return ret;
	}

	/*
	* Sets the blocklag, blockstun, and block animations of a HitResult before returning a copy of it.
	* 
	* If this function is not called, the default values for blocklag and blockstun are both 0, while
	* the default values for the animations are for the mid blockstun animations.
	*/
	HitResult block(int blocklag, int blockstun) {
		HitResult ret = *this;
		ret.blocklag = blocklag;
		ret.blockstun = blockstun;
		return ret;
	}

	/*
	* Sets the hit and block animations of a HitResult before returning a copy of it. These values 
	* determine what animation to put the opponent into when they are put into standard ground hit or 
	* blockstun. They are not used for standard air hitstun or any other form of damage. In addition, 
	* the opponent automatically goes into a unique hitstun animation on critical hit.
	*
	* If this function is not called, the default values for these animations are the light mid hitstun
	* animations.
	*/
	HitResult anims(std::string stand_hit, std::string crouch_hit, std::string stand_block, std::string crouch_block) {
		HitResult ret = *this;
		ret.stand_hit_anim = "stand_hitstun_" + stand_hit;
		ret.crouch_hit_anim = "crouch_hitstun_" + crouch_hit;
		ret.stand_block_anim = "stand_blockstun_" + stand_block;
		ret.crouch_block_anim = "crouch_blockstun_" + crouch_block;
		return ret;
	}

	int hitlag;
	int hitstun;
	int crit_hitlag;
	int crit_hitstun;
	int blocklag;
	int blockstun;

	float base_damage;
	float chip_percent;

	float meter_gain;

	float damage_scale_add;
	float damage_scale_combo_start;

	int juggle_start;
	int juggle_min;
	int juggle_increase;
	int juggle_max;

	std::string stand_hit_anim;
	std::string crouch_hit_anim;
	std::string stand_block_anim;
	std::string crouch_block_anim;
};

struct HitMove {
	HitMove() {
		pushback_ground_hit = 0.0f;
		pushback_ground_block = 0.0f;
		pushback_air_x = 0.0f;
		pushback_air_y = 0.0f;
		pushback_frames = 0;
		pushback_counter_mul = 1.0f;
		pushback_punish_mul = 1.0f;
		launch_init_y = 0.0f;
		launch_gravity = 0.0f;
		launch_max_fall = 0.0f;
		launch_x = 0.0f;
		target_x = 0.0f;
		target_y = 0.0f;
		target_frames = 0.0f;
	}

	HitMove ground(float hit, float block) {
		HitMove ret = *this;
		ret.pushback_ground_hit = hit * 10.0f;
		ret.pushback_ground_block = block * 10.0f;
		return ret;
	}

	HitMove air(float x, float y) {
		HitMove ret = *this;
		ret.pushback_air_x = x * 10.0f;
		ret.pushback_air_y = y * 10.0f;
		return ret;
	}

	HitMove frames(int frames) {
		HitMove ret = *this;
		ret.pushback_frames = frames;
		return ret;
	}

	HitMove counter(float mul) {
		HitMove ret = *this;
		ret.pushback_counter_mul = mul;
		return ret;
	}

	HitMove punish(float mul) {
		HitMove ret = *this;
		ret.pushback_punish_mul = mul;
		return ret;
	}

	HitMove launch(float init_y, float gravity, float max_fall, float x) {
		HitMove ret = *this;
		ret.launch_init_y = init_y;
		ret.launch_gravity = gravity;
		ret.launch_max_fall = max_fall;
		ret.launch_x = x;
		return ret;
	}

	HitMove target_pos(float target_x, float target_y, int target_frames) {
		HitMove ret = *this;
		ret.target_x = target_x;
		ret.target_y = target_y;
		ret.target_frames = target_frames;
		return ret;
	}

	float pushback_ground_hit;
	float pushback_ground_block;
	float pushback_air_x;
	float pushback_air_y;
	int pushback_frames;
	float pushback_counter_mul;
	float pushback_punish_mul;

	float launch_init_y;
	float launch_gravity;
	float launch_max_fall;
	float launch_x;

	float target_x;
	float target_y;
	int target_frames;
};

enum HitFlag {
	HIT_FLAG_NONE = 0,

	HIT_FLAG_FORCE_STAND = 1,
	HIT_FLAG_FORCE_CROUCH = 2,
	HIT_FLAG_FORCE_AERIAL = 3,
	HIT_FLAG_KNOCKDOWN_FACE_DOWN = 4,
	HIT_FLAG_HARD_KNOCKDOWN = 8,
	HIT_FLAG_CONTINUE_LAUNCH = 16,
	HIT_FLAG_DISABLE_HITSTUN_PARRY = 32,
	HIT_FLAG_CRUMPLE_ON_KO = 64,

	HIT_FLAG_MAX = 255,
};

inline HitFlag operator|(HitFlag a, HitFlag b) {
	return static_cast<HitFlag>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum CriticalCondition {
	CRITICAL_CONDITION_NONE = 0,
	CRITICAL_CONDITION_COUNTERHIT = 1,
	CRITICAL_CONDITION_PUNISH = 2,
	CRITICAL_CONDITION_DODGE_COUNTER = 4,

	CRITICAL_CONDITION_MAX = 255,
};

inline CriticalCondition operator|(CriticalCondition a, CriticalCondition b) {
	return static_cast<CriticalCondition>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum DamageKind {
	DAMAGE_KIND_NORMAL,
	DAMAGE_KIND_NO_KO,
	DAMAGE_KIND_CHIP,
	DAMAGE_KIND_CHIP_KO,

	DAMAGE_KIND_MAX,
};

enum GrabboxKind : unsigned char {
	GRABBOX_KIND_NORMAL = 0,
	GRABBOX_KIND_NOTECH = 1,
	GRABBOX_KIND_HITSTUN = 2,

	GRABBOX_KIND_MAX = 255,
};

inline GrabboxKind operator|(GrabboxKind a, GrabboxKind b) {
	return static_cast<GrabboxKind>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum HurtboxKind {
	HURTBOX_KIND_NORMAL,
	HURTBOX_KIND_COUNTER,
	HURTBOX_KIND_RIGHT_OF_WAY,

	HURTBOX_KIND_MAX,
};

enum IntangibleKind : unsigned char {
	INTANGIBLE_KIND_NONE = 0,
	INTANGIBLE_KIND_HIGH = 1,
	INTANGIBLE_KIND_MID = 2,
	INTANGIBLE_KIND_LOW = 4,
	INTANGIBLE_KIND_GROUND = 8,
	INTANGIBLE_KIND_AERIAL = 16,
	INTANGIBLE_KIND_THROW = 32,
	INTANGIBLE_KIND_PROJECTILE = 64,
	INTANGIBLE_KIND_INVINCIBLE = 128,

	INTANGIBLE_KIND_MAX = 255,
};

inline IntangibleKind operator|(IntangibleKind a, IntangibleKind b) {
	return static_cast<IntangibleKind>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum IncomingCollisionKind {
	INCOMING_COLLISION_KIND_NONE,
	INCOMING_COLLISION_KIND_HIT,
	INCOMING_COLLISION_KIND_BLOCKED,
	INCOMING_COLLISION_KIND_PARRIED,
	INCOMING_COLLISION_KIND_HITSTUN_PARRIED,
	INCOMING_COLLISION_KIND_ARMORED,
	INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED,
	INCOMING_COLLISION_KIND_INVINCIBILITY,
	INCOMING_COLLISION_KIND_COUNTER,
	INCOMING_COLLISION_KIND_MAX
};