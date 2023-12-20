#pragma once

#define HITBOX_COUNT_MAX 9

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

enum HitLevel {
	HIT_LEVEL_LIGHT,
	HIT_LEVEL_MEDIUM,
	HIT_LEVEL_HEAVY,

	HIT_LEVEL_MAX,
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

enum HitFlag {
	HIT_FLAG_NONE = 0,

	HIT_FLAG_FORCE_STAND = 1,
	HIT_FLAG_FORCE_CROUCH = 2,
	HIT_FLAG_FORCE_AERIAL = 4,
	HIT_FLAG_KNOCKDOWN_FACE_DOWN = 8,
	HIT_FLAG_HARD_KNOCKDOWN = 16,
	HIT_FLAG_CONTINUE_LAUNCH = 32,
	HIT_FLAG_DISABLE_HITSTUN_PARRY = 64,

	HIT_FLAG_MAX = 255,
};

inline HitFlag operator|(HitFlag a, HitFlag b) {
	return static_cast<HitFlag>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

enum SpecialStatusCondition {
	SPECIAL_STATUS_CONDITION_NONE = 0,
	SPECIAL_STATUS_CONDITION_COUNTERHIT = 1,
	SPECIAL_STATUS_CONDITION_PUNISH = 2,
	SPECIAL_STATUS_CONDITION_JUMP_COUNTERHIT = 4,

	SPECIAL_STATUS_CONDITION_MAX = 255,
};

inline SpecialStatusCondition operator|(SpecialStatusCondition a, SpecialStatusCondition b) {
	return static_cast<SpecialStatusCondition>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
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