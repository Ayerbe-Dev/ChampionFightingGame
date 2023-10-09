#pragma once

template <typename T> class TargetVar {
public:
	TargetVar<T>() {
		val = T();
		target_val = T();
		target_change_per_frame = T();
		frames = 0;
	}

	~TargetVar<T>() {

	}

	TargetVar<T>(T& other) {
		if (val != other) {
			val = other;
			frames = 0;
		}
	}

	TargetVar<T>(const T& other) {
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

	void process() {
		if (frames) {
			val += target_change_per_frame;
			frames--;
			if (!frames) {
				val = target_val;
			}
		}
	}
	
	void set_target_val(T target, int frames) {
		if (target_val == this->target_val && frames == this->frames - 1) return;
		this->target_val = target_val;
		target_change_per_frame = target_val - val;
		target_change_per_frame /= frames;
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
	T val;
	T target_val;
	T target_change_per_frame;
	unsigned int frames;
};