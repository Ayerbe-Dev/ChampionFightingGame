#pragma once
#include "TargetVar.h"
#include "glm/glm.hpp"

/// <summary>
/// A TargetVar equivalent of glm::vec3, where each element of the vec3 is a TargetVar of a float instead of
/// a float. Unlike TargetVars of glm::vec3, this class can give a separate target value to each component
/// of the vec3.
/// </summary>
class TargetVec3 {
public:
	TargetVec3() {
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}

	operator glm::vec3() const {
		return glm::vec3(x, y, z);
	}

	TargetVec3& operator=(const glm::vec3& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}

	TargetVec3& operator=(const TargetVec3& rhs) {
		if (this != &rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
		}
		return *this;
	}

	TargetVec3& operator+= (const glm::vec3& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	TargetVec3& operator+= (const TargetVec3& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	TargetVec3& operator-= (const glm::vec3& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	TargetVec3& operator-= (const TargetVec3& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	TargetVec3& operator*= (const glm::vec3& rhs) {
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		return *this;
	}

	TargetVec3& operator*= (const TargetVec3& rhs) {
		this->x *= rhs.x;
		this->y *= rhs.y;
		this->z *= rhs.z;
		return *this;
	}

	TargetVec3& operator/= (const glm::vec3& rhs) {
		this->x /= rhs.x;
		this->y /= rhs.y;
		this->z /= rhs.z;
		return *this;
	}

	TargetVec3& operator/= (const TargetVec3& rhs) {
		this->x /= rhs.x;
		this->y /= rhs.y;
		this->z /= rhs.z;
		return *this;
	}

	bool operator==(const glm::vec3& rhs) {
		return (glm::vec3)(*this) == rhs;
	}

	bool operator!=(const glm::vec3& rhs) {
		return (glm::vec3)(*this) != rhs;
	}

	void set_target_val(glm::vec3 target, int frames) {
		x.set_target_val(target.x, frames);
		y.set_target_val(target.y, frames);
		z.set_target_val(target.z, frames);
	}

	TargetVar<float> x;
	TargetVar<float> y;
	TargetVar<float> z;
};

/// <summary>
/// A TargetVar equivalent of glm::vec2, where each element of the vec2 is a TargetVar of a float instead of
/// a float. Unlike TargetVars of glm::vec2, this class can give a separate target value to each component
/// of the vec2.
/// </summary>
class TargetVec2 {
public:
	TargetVec2() {
		x = 0.0;
		y = 0.0;
	}

	operator glm::vec2() const {
		return glm::vec2(x, y);
	}

	TargetVec2& operator=(const glm::vec2& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		return *this;
	}

	TargetVec2& operator=(const TargetVec2& rhs) {
		if (this != &rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
		}
		return *this;
	}

	TargetVec2& operator+= (const glm::vec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	TargetVec2& operator+= (const TargetVec2& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	TargetVec2& operator-= (const glm::vec2& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	TargetVec2& operator-= (const TargetVec2& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	TargetVec2& operator*= (const glm::vec2& rhs) {
		this->x *= rhs.x;
		this->y *= rhs.y;
		return *this;
	}

	TargetVec2& operator*= (const TargetVec2& rhs) {
		this->x *= rhs.x;
		this->y *= rhs.y;
		return *this;
	}

	TargetVec2& operator/= (const glm::vec2& rhs) {
		this->x /= rhs.x;
		this->y /= rhs.y;
		return *this;
	}

	TargetVec2& operator/= (const TargetVec2& rhs) {
		this->x /= rhs.x;
		this->y /= rhs.y;
		return *this;
	}

	bool operator==(const glm::vec2& rhs) {
		return (glm::vec2)(*this) == rhs;
	}

	bool operator!=(const glm::vec2& rhs) {
		return (glm::vec2)(*this) != rhs;
	}

	void set_target_val(glm::vec2 target, int frames) {
		x.set_target_val(target.x, frames);
		y.set_target_val(target.y, frames);
	}

	TargetVar<float> x;
	TargetVar<float> y;
};