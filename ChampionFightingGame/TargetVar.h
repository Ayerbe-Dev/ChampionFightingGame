#pragma once
#include "TargetVarManager.h"

class BaseTargetVar {
public:
	BaseTargetVar() {
		TargetVarManager::get_instance()->register_target_var(this);
	}
	virtual	~BaseTargetVar() {
		TargetVarManager::get_instance()->unregister_target_var(this);
	}

	virtual void process() {}
};

template <class T> class TargetVar : public BaseTargetVar {
public:
	TargetVar() = default;


	TargetVar(T& other) {
		if (val != other) {
			val = other;
			frames = 0;
		}
	}
	
	TargetVar(const T& other) {
		if (val != other) {
			val = other;
			frames = 0;
		}
	}

	template <typename U>
	T operator+(const U& rhs) {
		return val + rhs;
	}

	template <typename U>
	T operator-(const U& rhs) {
		return val - rhs;
	}

	template <typename U>
	T operator*(const U& rhs) {
		return val * rhs;
	}

	template <typename U>
	T operator/(const U& rhs) {
		return val / rhs;
	}

	template <typename U>
	TargetVar<T>& operator=(const U& rhs) {
		val = (T)rhs;
		frames = 0;
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator+=(const U& rhs) {
		val += rhs;
		frames = 0;
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator-=(const U& rhs) {
		val -= rhs;
		frames = 0;
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator*=(const U& rhs) {
		val *= rhs;
		frames = 0;
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator/=(const U& rhs) {
		val /= rhs;
		frames = 0;
		return *this;
	}

	template <typename U>
	bool operator==(const U& rhs) {
		return val == rhs;
	}

	template <typename U>
	bool operator!=(const U& rhs) {
		return val != rhs;
	}

	template <typename U>
	T operator+(const TargetVar<U>& rhs) {
		return val + rhs.get_val();
	}

	template <typename U>
	T operator-(const TargetVar<U>& rhs) {
		return val - rhs.get_val();
	}

	template <typename U>
	T operator*(const TargetVar<U>& rhs) {
		return val * rhs.get_val();
	}

	template <typename U>
	T operator/(const TargetVar<U>& rhs) {
		return val / rhs.get_val();
	}

	template <typename U>
	TargetVar<T>& operator=(const TargetVar<U>& rhs) {
		if (this != &rhs) {
			val = rhs.get_val();
			frames = 0;
		}
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator+=(const TargetVar<U>& rhs) {
		if (this != &rhs) {
			val += rhs.get_val();
			frames = 0;
		}
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator-=(const TargetVar<U>& rhs) {
		if (this != &rhs) {
			val -= rhs.get_val();
			frames = 0;
		}
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator*=(const TargetVar<U>& rhs) {
		if (this != &rhs) {
			val *= rhs.get_val();
			frames = 0;
		}
		return *this;
	}

	template <typename U>
	TargetVar<T>& operator/=(const TargetVar<U>& rhs) {
		if (this != &rhs) {
			val /= rhs.get_val();
			frames = 0;
		}
		return *this;
	}

	template <typename U>
	bool operator==(const TargetVar<U>& rhs) {
		return val == rhs.get_val();
	}

	template <typename U>
	bool operator!=(const TargetVar<U>& rhs) {
		return val != rhs.get_val();
	}

	void process() override {
		if (frames) {
			val += target_change_per_frame;
			frames--;
			if (!frames) {
				val = target_val;
			}
		}
	}
	
	void set_target_val(T target_val, unsigned int frames) {
		if (target_val == this->target_val) return;
		this->target_val = target_val;
		target_change_per_frame = target_val - val;
		target_change_per_frame /= frames;
		if (target_change_per_frame == T(0)) {
			target_change_per_frame = val - target_val;
			target_change_per_frame /= frames;
			target_change_per_frame *= -1;
		}
		this->frames = frames;
	}

	void set_val(const T& val) {
		this->val = val;
	}

	T get_val() const {
		return val;
	}

	T* get_val_addr() const {
		return &val;
	}

	T get_target_change_per_frame() const {
		return target_change_per_frame;
	}

	unsigned int get_frames() const {
		return frames;
	}
private:
	T val{};
	T target_val{};
	T target_change_per_frame{};
	unsigned int frames = 0;
};