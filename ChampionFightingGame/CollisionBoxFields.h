#pragma once

enum CollisionKind : unsigned char {
	COLLISION_KIND_GROUND = 1,
	COLLISION_KIND_AIR = 2,
	COLLISION_KIND_DOWN = 4,
	COLLISION_KIND_PROJECTILE = 8,
	COLLISION_KIND_SOFT_INTANGIBLE = 16,
	COLLISION_KIND_ARMOR = 32,

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
		stand_hit_anim = "stand_hitstun_light_mid";
		crouch_hit_anim = "crouch_hitstun_light_mid";
		stand_block_anim = "stand_blockstun_mid";
		crouch_block_anim = "crouch_blockstun_mid";
	}

	HitResult& ground(float hit, float block) {
		this->pushback_ground_hit = hit * 10.0f;
		this->pushback_ground_block = block * 10.0f;
		return *this;
	}

	HitResult& air(float x, float y) {
		this->pushback_air_x = x * 10.0f;
		this->pushback_air_y = y * 10.0f;
		return *this;
	}

	HitResult& frames(int frames) {
		this->pushback_frames = frames;
		return *this;
	}

	HitResult& counter(float mul) {
		this->pushback_counter_mul = mul;
		return *this;
	}

	HitResult& punish(float mul) {
		this->pushback_punish_mul = mul;
		return *this;
	}

	HitResult& launch(float init_y, float gravity, float max_fall, float x) {
		this->launch_init_y = init_y;
		this->launch_gravity = gravity;
		this->launch_max_fall = max_fall;
		this->launch_x = x;
		return *this;
	}

	HitResult& target_pos(float target_x, float target_y, int target_frames) {
		this->target_x = target_x;
		this->target_y = target_y;
		this->target_frames = target_frames;
		return *this;
	}

	HitResult& hitstun(std::string stand, std::string crouch) {
		this->stand_hit_anim = stand;
		this->crouch_hit_anim = crouch;
		return *this;
	}

	HitResult& blockstun(std::string stand, std::string crouch) {
		this->stand_block_anim = stand;
		this->crouch_block_anim = crouch;
		return *this;
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

	std::string stand_hit_anim;
	std::string crouch_hit_anim;
	std::string stand_block_anim;
	std::string crouch_block_anim;
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
	CRITICAL_CONDITION_JUMP_COUNTERHIT = 4,

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
	INTANGIBLE_KIND_AERIAL = 8,
	INTANGIBLE_KIND_THROW = 16,
	INTANGIBLE_KIND_PROJECTILE = 32,
	INTANGIBLE_KIND_INVINCIBLE = 64,
	INTANGIBLE_KIND_SOFT = 128,

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