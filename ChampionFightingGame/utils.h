#pragma once
#include <iostream>

#define FORCE_INLINE __forceinline
#define NO_INLINE __declspec(noinline)

int clamp(int min, int value, int max);
float clampf(float min, float value, float max);
std::string filter_string(const std::string& to, const std::string& remove);
int get_blank(std::string s);
float get_relative_one_percent(float val, float denom);
int round_up_odd(int val);
int rng(const int& min, const int& max);
float rng_f(const float& min, const float& max);
float lerp(float a, float b, float f);
bool is_between(float a, float mid, float b);
std::int32_t convert_to_int(char* buffer, std::size_t len);
std::string float_to_string(float val, int precision);